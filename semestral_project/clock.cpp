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
    clock_obj.rotationAngleLocation = glGetUniformLocation(clock_obj.shaderProgram, "rotationAngle");
    clock_obj.useRotationLocation = glGetUniformLocation(clock_obj.shaderProgram, "useRotation");

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

void drawClock() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(clock_obj.shaderProgram);

    // position on back wall
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(7.9f, 1.0f, -5.0f));
    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));
    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * model;
    glUniformMatrix4fv(clock_obj.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

    // draw clock face
    glUniform1i(clock_obj.useRotationLocation, 0);
    glUniform1f(clock_obj.rotationAngleLocation, 0.0f);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, clock_obj.clockTexture);
    glUniform1i(clock_obj.texSamplerLocation, 0);
    glBindVertexArray(clock_obj.vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // get time for rotation
    float timeMs = (float)glutGet(GLUT_ELAPSED_TIME);
    float secondAngle = (timeMs / 1000.0f) * (360.0f / 60.0f);   // one rotation per minute
    float minuteAngle = (timeMs / 60000.0f) * (360.0f / 60.0f);  // one rotation per hour
    float hourAngle = (timeMs / 3600000.0f) * (360.0f / 12.0f); // one rotation per 12 hours

    // draw second arrow
    glm::mat4 secondModel = glm::translate(glm::mat4(1.0f), glm::vec3(7.85f, 1.0f, -5.0f));
    secondModel = glm::rotate(secondModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    secondModel = glm::scale(secondModel, glm::vec3(2.0f, 2.0f, 1.0f));
    glm::mat4 secondPVM = getProjectionMatrix() * getViewMatrix() * secondModel;
    glUniformMatrix4fv(clock_obj.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(secondPVM));
    glUniform1i(clock_obj.useRotationLocation, 1);
    glUniform1f(clock_obj.rotationAngleLocation, -secondAngle);
    glBindTexture(GL_TEXTURE_2D, clock_obj.arrowTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // draw minute arrow (slightly smaller)
    glm::mat4 minuteModel = glm::translate(glm::mat4(1.0f), glm::vec3(7.85f, 1.0f, -5.0f));
    minuteModel = glm::rotate(minuteModel, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    minuteModel = glm::scale(minuteModel, glm::vec3(1.6f, 1.6f, 1.0f));
    glm::mat4 minutePVM = getProjectionMatrix() * getViewMatrix() * minuteModel;
    glUniformMatrix4fv(clock_obj.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(minutePVM));
    glUniform1f(clock_obj.rotationAngleLocation, -minuteAngle);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
    glDisable(GL_BLEND);
}