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

    float timeMs = (float)glutGet(GLUT_ELAPSED_TIME);
    float secondAngle = (timeMs / 1000.0f) * (360.0f / 60.0f);
    float minuteAngle = (timeMs / 60000.0f) * (360.0f / 60.0f);

    // draw clock face
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(7.9f, 1.0f, -5.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));
    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * model;
    glUniformMatrix4fv(clock_obj.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

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

    glUniform1i(clock_obj.useRotationLocation, 1);
    glm::mat4 minuteTexMat = buildArrowMatrix(-minuteAngle);
    glUniformMatrix4fv(clock_obj.texMatrixLocation, 1, GL_FALSE, glm::value_ptr(minuteTexMat));

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glDisable(GL_BLEND);
}