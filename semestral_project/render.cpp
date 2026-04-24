#include "render.h"
#include "camera.h"
#include <iostream>
#include "board.h"

RoomGeometry room;

bool spotLightOn = false;

// 6 faces, 2 triangles each, 6 vertices per face
// format: x, y, z, nx, ny, nz, u, v
static const float roomVertices[] = {
    // Floor (y = -1), normal pointing up
     -1,-1,-1,  0,1,0,  0,0,
     1,-1,-1,  0,1,0,  1,0,
     1,-1, 1,  0,1,0,  1,1,
     -1,-1,-1,  0,1,0,  0,0,
     1,-1, 1,  0,1,0,  1,1,
     -1,-1, 1,  0,1,0,  0,1,

    // Ceiling (y = 1), normal pointing down
    -1, 1,-1,  0,-1,0,  0,0,
    1, 1, 1,  0,-1,0,  1,1,
    1, 1,-1,  0,-1,0,  1,0,
    -1, 1,-1,  0,-1,0,  0,0,
    -1, 1, 1,  0,-1,0,  0,1,
    1, 1, 1,  0,-1,0,  1,1,

     // Back wall (z = -1), normal pointing forward
     -1,-1,-1,  0,0,1,  0,0,
     1, 1,-1,  0,0,1,  1,1,
     1,-1,-1,  0,0,1,  1,0,
     -1,-1,-1,  0,0,1,  0,0,
     -1, 1,-1,  0,0,1,  0,1,
     1, 1,-1,  0,0,1,  1,1,

      // Front wall (z = 1), normal pointing backward
      -1,-1, 1,  0,0,-1,  0,0,
      1,-1, 1,  0,0,-1,  1,0,
      1, 1, 1,  0,0,-1,  1,1,
      -1,-1, 1,  0,0,-1,  0,0,
      1, 1, 1,  0,0,-1,  1,1,
      -1, 1, 1,  0,0,-1,  0,1,

      // Left wall (x = -1), normal pointing right
      -1,-1,-1,  1,0,0,  0,0,
      -1,-1, 1,  1,0,0,  1,0,
      -1, 1, 1,  1,0,0,  1,1,
      -1,-1,-1,  1,0,0,  0,0,
      -1, 1, 1,  1,0,0,  1,1,
      -1, 1,-1,  1,0,0,  0,1,

      // Right wall (x = 1), normal pointing left
       1,-1,-1,  -1,0,0,  0,0,
       1, 1, 1,  -1,0,0,  1,1,
       1,-1, 1,  -1,0,0,  1,0,
       1,-1,-1,  -1,0,0,  0,0,
       1, 1,-1,  -1,0,0,  0,1,
       1, 1, 1,  -1,0,0,  1,1,
};

void initScene() {
     GLuint shaders[] = {
     pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/room.vert"),
     pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/room.frag"),
     0  
    };

    room.shaderProgram = pgr::createProgram(shaders);
    room.dirLightDirLocation = glGetUniformLocation(room.shaderProgram, "dirLightDir");
    room.dirLightColorLocation = glGetUniformLocation(room.shaderProgram, "dirLightColor");
    room.pointLightPosLocation = glGetUniformLocation(room.shaderProgram, "pointLightPos");
    room.pointLightColorLocation = glGetUniformLocation(room.shaderProgram, "pointLightColor");
    room.pointLightConstantLocation = glGetUniformLocation(room.shaderProgram, "pointLightConstant");
    room.pointLightLinearLocation = glGetUniformLocation(room.shaderProgram, "pointLightLinear");
    room.pointLightQuadraticLocation = glGetUniformLocation(room.shaderProgram, "pointLightQuadratic");
    room.spotLightPosLocation = glGetUniformLocation(room.shaderProgram, "spotLightPos");
    room.spotLightDirLocation = glGetUniformLocation(room.shaderProgram, "spotLightDir");
    room.spotLightColorLocation = glGetUniformLocation(room.shaderProgram, "spotLightColor");
    room.spotLightCutoffLocation = glGetUniformLocation(room.shaderProgram, "spotLightCutoff");
    room.spotLightOuterCutoffLocation = glGetUniformLocation(room.shaderProgram, "spotLightOuterCutoff");
    room.MmatrixLocation = glGetUniformLocation(room.shaderProgram, "Mmatrix");
    room.normalMatrixLocation = glGetUniformLocation(room.shaderProgram, "normalMatrix");
    room.shininessLocation = glGetUniformLocation(room.shaderProgram, "shininess");
    room.cameraPosLocation = glGetUniformLocation(room.shaderProgram, "cameraPos");

    // get attribute locations
    room.posLocation = glGetAttribLocation(room.shaderProgram, "position");
    room.normalLocation = glGetAttribLocation(room.shaderProgram, "normal");
    room.PVMmatrixLocation = glGetUniformLocation(room.shaderProgram, "PVMmatrix");

    // create VAO + VBO
    glGenVertexArrays(1, &room.vao);
    glBindVertexArray(room.vao);

    glGenBuffers(1, &room.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, room.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roomVertices), roomVertices, GL_STATIC_DRAW);

    // position: first 3 floats
    glEnableVertexAttribArray(room.posLocation);
    glVertexAttribPointer(room.posLocation, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)0);

    // normal: next 3 floats
    glEnableVertexAttribArray(room.normalLocation);
    glVertexAttribPointer(room.normalLocation, 3, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)(3 * sizeof(float)));

    // texcoord attributes
    GLint texCoordLocation = glGetAttribLocation(room.shaderProgram, "texCoord");
    glEnableVertexAttribArray(texCoordLocation);
    glVertexAttribPointer(texCoordLocation, 2, GL_FLOAT, GL_FALSE,
        8 * sizeof(float), (void*)(6 * sizeof(float)));

    // load textures
    room.floorTexture = pgr::createTexture("textures/floor_texture.jpg");
    room.wallTexture = pgr::createTexture("textures/wall_texture.jpg");
    room.ceilingTexture = pgr::createTexture("textures/ceiling_texture.jpg");
    room.texSamplerLocation = glGetUniformLocation(room.shaderProgram, "texSampler");
    room.wallSamplerLocation = glGetUniformLocation(room.shaderProgram, "wallSampler");
    room.ceilingSamplerLocation = glGetUniformLocation(room.shaderProgram, "ceilingSampler");

    glBindVertexArray(0);

    initBoard();
}

void drawScene() {
    glUseProgram(room.shaderProgram);

    glm::vec3 camPos = getCameraPos();
    glm::vec3 camFront = getCameraFront();

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(8.0f, 3.5f, 10.0f));
    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * model;
    glm::mat4 normalMat = glm::transpose(glm::inverse(model));

    glUniformMatrix4fv(room.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
    glUniformMatrix4fv(room.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(room.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMat));

    // directional light
    glUniform3f(room.dirLightDirLocation, 0.5f, 1.0f, 0.3f);
    glUniform3f(room.dirLightColorLocation, 1.0f, 0.95f, 0.8f);

    // point light
    glUniform3f(room.pointLightPosLocation, 0.0f, 3.0f, 0.0f);
    glUniform3f(room.pointLightColorLocation, 1.0f, 0.9f, 0.7f);
    glUniform1f(room.pointLightConstantLocation, 1.0f);
    glUniform1f(room.pointLightLinearLocation, 0.09f);
    glUniform1f(room.pointLightQuadraticLocation, 0.032f);

    // spot light
    glUniform3fv(room.spotLightPosLocation, 1, glm::value_ptr(camPos));
    glUniform3fv(room.spotLightDirLocation, 1, glm::value_ptr(camFront));
    glm::vec3 spotColor = spotLightOn ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(0.0f);
    glUniform3fv(room.spotLightColorLocation, 1, glm::value_ptr(spotColor));
    glUniform1f(room.spotLightCutoffLocation, glm::cos(glm::radians(12.5f)));
    glUniform1f(room.spotLightOuterCutoffLocation, glm::cos(glm::radians(17.5f)));

    glUniform1f(room.shininessLocation, 16.0f);
    glUniform3fv(room.cameraPosLocation, 1, glm::value_ptr(camPos));

    // textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, room.floorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, room.wallTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, room.ceilingTexture);

    glUniform1i(room.texSamplerLocation, 0);
    glUniform1i(room.wallSamplerLocation, 1);
    glUniform1i(room.ceilingSamplerLocation, 2);

    glBindVertexArray(room.vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    drawBoard();
}

void updateScene() {
}