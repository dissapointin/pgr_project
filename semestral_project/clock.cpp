#include "clock.h"
#include "camera.h"

ClockGeometry clock_obj;

// simple quad: position + texcoord
static const float quadVertices[] = {
    // x     y     z     u     v
    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
};

void initClock() {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/clock.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/clock.frag"),
        0
    };
    clock_obj.shaderProgram = pgr::createProgram(shaders);

    clock_obj.posLocation = glGetAttribLocation(clock_obj.shaderProgram, "position");
    clock_obj.texCoordLocation = glGetAttribLocation(clock_obj.shaderProgram, "texCoord");
    clock_obj.PVMmatrixLocation = glGetUniformLocation(clock_obj.shaderProgram, "PVMmatrix");
    clock_obj.texSamplerLocation = glGetUniformLocation(clock_obj.shaderProgram, "texSampler");
    clock_obj.useRotationLocation = glGetUniformLocation(clock_obj.shaderProgram, "useRotation");
    clock_obj.texMatrixLocation = glGetUniformLocation(clock_obj.shaderProgram, "texMatrix");
    clock_obj.MmatrixLocation = glGetUniformLocation(clock_obj.shaderProgram, "Mmatrix");
    clock_obj.cameraPosLocation = glGetUniformLocation(clock_obj.shaderProgram, "cameraPos");
    clock_obj.fogEnabledLocation = glGetUniformLocation(clock_obj.shaderProgram, "fogEnabled");
    clock_obj.fogStartLocation = glGetUniformLocation(clock_obj.shaderProgram, "fogStart");
    clock_obj.fogEndLocation = glGetUniformLocation(clock_obj.shaderProgram, "fogEnd");
    clock_obj.fogColorLocation = glGetUniformLocation(clock_obj.shaderProgram, "fogColor");

    glGenVertexArrays(1, &clock_obj.vao);
    glBindVertexArray(clock_obj.vao);

    glGenBuffers(1, &clock_obj.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, clock_obj.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(clock_obj.posLocation);
    glVertexAttribPointer(clock_obj.posLocation, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(clock_obj.texCoordLocation);
    glVertexAttribPointer(clock_obj.texCoordLocation, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    clock_obj.clockTexture = pgr::createTexture("textures/clock.png");
    clock_obj.arrowTexture = pgr::createTexture("textures/arrow.png");
}

/// @brief Build rotation matrix around center 
/// @param angle rotation angle in degrees
/// @return combined transformation matrix
static glm::mat4 buildArrowMatrix(float angle) {
    // transformation 1: rotate
    glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    // transformation 2: slight scale 
    mat = glm::scale(mat, glm::vec3(0.8f, 0.8f, 0.8f));
    return mat;
}

void drawClock() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(clock_obj.shaderProgram);

    extern bool fogEnabled;
    glm::vec3 camPos = getCameraPos();
    glUniform3fv(clock_obj.cameraPosLocation, 1, glm::value_ptr(camPos));
    glUniform1i(clock_obj.fogEnabledLocation, fogEnabled ? 1 : 0);
    glUniform1f(clock_obj.fogStartLocation, 1.0f);
    glUniform1f(clock_obj.fogEndLocation, 10.0f);
    glUniform3f(clock_obj.fogColorLocation, 0.7f, 0.7f, 0.7f);

    float timeMs = (float)glutGet(GLUT_ELAPSED_TIME);
    float secondAngle = (timeMs / 1000.0f) * (360.0f / 60.0f);
    float minuteAngle = (timeMs / 60000.0f) * (360.0f / 60.0f);

    // draw clock face
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(7.9f, 1.0f, -5.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));
    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * model;
    glUniformMatrix4fv(clock_obj.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
    glUniformMatrix4fv(clock_obj.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(model));

    glUniform1i(clock_obj.useRotationLocation, 0);
    glm::mat4 identityMat = glm::mat4(1.0f);
    glUniformMatrix4fv(clock_obj.texMatrixLocation, 1, GL_FALSE, glm::value_ptr(identityMat));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, clock_obj.clockTexture);
    glUniform1i(clock_obj.texSamplerLocation, 0);
    glBindVertexArray(clock_obj.vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // draw second arrow
    glm::mat4 secondModel = glm::translate(glm::mat4(1.0f), glm::vec3(7.85f, 1.0f, -5.0f));
    secondModel = glm::rotate(secondModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    secondModel = glm::scale(secondModel, glm::vec3(2.0f, 2.0f, 1.0f));
    glm::mat4 secondPVM = getProjectionMatrix() * getViewMatrix() * secondModel;
    glUniformMatrix4fv(clock_obj.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(secondPVM));
    glUniformMatrix4fv(clock_obj.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(secondModel));

    glUniform1i(clock_obj.useRotationLocation, 1);
    glm::mat4 secondTexMat = buildArrowMatrix(-secondAngle);
    glUniformMatrix4fv(clock_obj.texMatrixLocation, 1, GL_FALSE, glm::value_ptr(secondTexMat));

    glBindTexture(GL_TEXTURE_2D, clock_obj.arrowTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // draw minute arrow
    glm::mat4 minuteModel = glm::translate(glm::mat4(1.0f), glm::vec3(7.85f, 1.0f, -5.0f));
    minuteModel = glm::rotate(minuteModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    minuteModel = glm::scale(minuteModel, glm::vec3(1.6f, 1.6f, 1.0f));
    glm::mat4 minutePVM = getProjectionMatrix() * getViewMatrix() * minuteModel;
    glUniformMatrix4fv(clock_obj.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(minutePVM));
    glUniformMatrix4fv(clock_obj.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(minuteModel));

    glUniform1i(clock_obj.useRotationLocation, 1);
    glm::mat4 minuteTexMat = buildArrowMatrix(-minuteAngle);
    glUniformMatrix4fv(clock_obj.texMatrixLocation, 1, GL_FALSE, glm::value_ptr(minuteTexMat));

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glDisable(GL_BLEND);
}