#include "render.h"
#include "camera.h"
#include <iostream>

RoomGeometry room;

// 6 faces, 2 triangles each, 6 vertices per face
// format: x, y, z, nx, ny, nz
static const float roomVertices[] = {
    // Floor (y = -1), normal pointing up
    -1,-1,-1,  0, 1, 0,
     1,-1,-1,  0, 1, 0,
     1,-1, 1,  0, 1, 0,
    -1,-1,-1,  0, 1, 0,
     1,-1, 1,  0, 1, 0,
    -1,-1, 1,  0, 1, 0,

    // Ceiling (y = 1), normal pointing down
    -1, 1,-1,  0,-1, 0,
     1, 1, 1,  0,-1, 0,
     1, 1,-1,  0,-1, 0,
    -1, 1,-1,  0,-1, 0,
    -1, 1, 1,  0,-1, 0,
     1, 1, 1,  0,-1, 0,

     // Back wall (z = -1), normal pointing forward
     -1,-1,-1,  0, 0, 1,
      1, 1,-1,  0, 0, 1,
      1,-1,-1,  0, 0, 1,
     -1,-1,-1,  0, 0, 1,
     -1, 1,-1,  0, 0, 1,
      1, 1,-1,  0, 0, 1,

      // Front wall (z = 1), normal pointing backward
      -1,-1, 1,  0, 0,-1,
       1,-1, 1,  0, 0,-1,
       1, 1, 1,  0, 0,-1,
      -1,-1, 1,  0, 0,-1,
       1, 1, 1,  0, 0,-1,
      -1, 1, 1,  0, 0,-1,

      // Left wall (x = -1), normal pointing right
      -1,-1,-1,  1, 0, 0,
      -1,-1, 1,  1, 0, 0,
      -1, 1, 1,  1, 0, 0,
      -1,-1,-1,  1, 0, 0,
      -1, 1, 1,  1, 0, 0,
      -1, 1,-1,  1, 0, 0,

      // Right wall (x = 1), normal pointing left
       1,-1,-1, -1, 0, 0,
       1, 1, 1, -1, 0, 0,
       1,-1, 1, -1, 0, 0,
       1,-1,-1, -1, 0, 0,
       1, 1,-1, -1, 0, 0,
       1, 1, 1, -1, 0, 0,
};

void initScene() {
     GLuint shaders[] = {
     pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/room.vert"),
     pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/room.frag"),
     0  
    };

    room.shaderProgram = pgr::createProgram(shaders);

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
        6 * sizeof(float), (void*)0);

    // normal: next 3 floats
    glEnableVertexAttribArray(room.normalLocation);
    glVertexAttribPointer(room.normalLocation, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

void drawScene() {
    glUseProgram(room.shaderProgram);

    glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(8.0f, 3.5f, 10.0f));
    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * model;
    glUniformMatrix4fv(room.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

    glBindVertexArray(room.vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

void updateScene() {
}