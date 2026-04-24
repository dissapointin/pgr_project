#include "board.h"
#include "camera.h"

BoardGeometry board;

// format: x, y, z, nx, ny, nz
// board on back wall (z = -1 in room space)
// width: -0.8 to 0.8, height: 0.1 to 0.7

static const float boardVertices[] = {

    // MAIN SURFACE
    -0.8f, 0.1f, 0.01f,  0,0,1,
     0.8f, 0.1f, 0.01f,  0,0,1,
     0.8f, 0.7f, 0.01f,  0,0,1,
    -0.8f, 0.1f, 0.01f,  0,0,1,
     0.8f, 0.7f, 0.01f,  0,0,1,
    -0.8f, 0.7f, 0.01f,  0,0,1,
    // back face
    -0.8f, 0.1f, -0.01f,  0,0,-1,
     0.8f, 0.7f, -0.01f,  0,0,-1,
     0.8f, 0.1f, -0.01f,  0,0,-1,
    -0.8f, 0.1f, -0.01f,  0,0,-1,
    -0.8f, 0.7f, -0.01f,  0,0,-1,
     0.8f, 0.7f, -0.01f,  0,0,-1,

     // TOP FRAME
     -0.8f, 0.7f,  -0.02f,  0,1,0,
      0.8f, 0.7f,  -0.02f,  0,1,0,
      0.8f, 0.75f, -0.02f,  0,1,0,
     -0.8f, 0.7f,  -0.02f,  0,1,0,
      0.8f, 0.75f, -0.02f,  0,1,0,
     -0.8f, 0.75f, -0.02f,  0,1,0,

     // === BOTTOM FRAME ===
     -0.8f, 0.05f, -0.02f,  0,-1,0,
      0.8f, 0.1f,  -0.02f,  0,-1,0,
      0.8f, 0.05f, -0.02f,  0,-1,0,
     -0.8f, 0.05f, -0.02f,  0,-1,0,
     -0.8f, 0.1f,  -0.02f,  0,-1,0,
      0.8f, 0.1f,  -0.02f,  0,-1,0,

      // LEFT FRAME
      -0.85f, 0.05f, -0.02f,  -1,0,0,
      -0.80f, 0.05f, -0.02f,  -1,0,0,
      -0.80f, 0.75f, -0.02f,  -1,0,0,
      -0.85f, 0.05f, -0.02f,  -1,0,0,
      -0.80f, 0.75f, -0.02f,  -1,0,0,
      -0.85f, 0.75f, -0.02f,  -1,0,0,

      // RIGHT FRAME
       0.80f, 0.05f, -0.02f,  1,0,0,
       0.85f, 0.05f, -0.02f,  1,0,0,
       0.85f, 0.75f, -0.02f,  1,0,0,
       0.80f, 0.05f, -0.02f,  1,0,0,
       0.85f, 0.75f, -0.02f,  1,0,0,
       0.80f, 0.75f, -0.02f,  1,0,0,

       // CHALK SHELF
       // top face
       -0.8f, 0.08f, 0.02f,  0,1,0,
        0.8f, 0.08f, 0.02f,  0,1,0,
        0.8f, 0.08f, 0.07f,  0,1,0,
       -0.8f, 0.08f, 0.02f,  0,1,0,
        0.8f, 0.08f, 0.07f,  0,1,0,
       -0.8f, 0.08f, 0.07f,  0,1,0,
       // front face
       -0.8f, 0.05f, 0.07f,  0,0,1,
        0.8f, 0.05f, 0.07f,  0,0,1,
        0.8f, 0.08f, 0.07f,  0,0,1,
       -0.8f, 0.05f, 0.07f,  0,0,1,
        0.8f, 0.08f, 0.07f,  0,0,1,
       -0.8f, 0.08f, 0.07f,  0,0,1,
};

const int BOARD_VERTEX_COUNT = sizeof(boardVertices) / (6 * sizeof(float));

void initBoard() {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/room.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/board.frag"),
        0
    };
    board.shaderProgram = pgr::createProgram(shaders);

    board.posLocation = glGetAttribLocation(board.shaderProgram, "position");
    board.normalLocation = glGetAttribLocation(board.shaderProgram, "normal");
    board.PVMmatrixLocation = glGetUniformLocation(board.shaderProgram, "PVMmatrix");
    board.MmatrixLocation = glGetUniformLocation(board.shaderProgram, "Mmatrix");
    board.normalMatrixLocation = glGetUniformLocation(board.shaderProgram, "normalMatrix");
    board.shininessLocation = glGetUniformLocation(board.shaderProgram, "shininess");
    board.cameraPosLocation = glGetUniformLocation(board.shaderProgram, "cameraPos");
    board.dirLightDirLocation = glGetUniformLocation(board.shaderProgram, "dirLightDir");
    board.dirLightColorLocation = glGetUniformLocation(board.shaderProgram, "dirLightColor");
    board.pointLightPosLocation = glGetUniformLocation(board.shaderProgram, "pointLightPos");
    board.pointLightColorLocation = glGetUniformLocation(board.shaderProgram, "pointLightColor");
    board.pointLightConstantLocation = glGetUniformLocation(board.shaderProgram, "pointLightConstant");
    board.pointLightLinearLocation = glGetUniformLocation(board.shaderProgram, "pointLightLinear");
    board.pointLightQuadraticLocation = glGetUniformLocation(board.shaderProgram, "pointLightQuadratic");
    board.spotLightPosLocation = glGetUniformLocation(board.shaderProgram, "spotLightPos");
    board.spotLightDirLocation = glGetUniformLocation(board.shaderProgram, "spotLightDir");
    board.spotLightColorLocation = glGetUniformLocation(board.shaderProgram, "spotLightColor");
    board.spotLightCutoffLocation = glGetUniformLocation(board.shaderProgram, "spotLightCutoff");
    board.spotLightOuterCutoffLocation = glGetUniformLocation(board.shaderProgram, "spotLightOuterCutoff");

    glGenVertexArrays(1, &board.vao);
    glBindVertexArray(board.vao);

    glGenBuffers(1, &board.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, board.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boardVertices), boardVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(board.posLocation);
    glVertexAttribPointer(board.posLocation, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(board.normalLocation);
    glVertexAttribPointer(board.normalLocation, 3, GL_FLOAT, GL_FALSE,
        6 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);
}

void drawBoard() {
    glUseProgram(board.shaderProgram);

    glm::vec3 camPos = getCameraPos();
    glm::vec3 camFront = getCameraFront();

    // position on back wall, scaled to room size
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, -9.7f));
    model = glm::scale(model, glm::vec3(8.0f, 5.0f, 1.0f));

    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * model;
    glm::mat4 normalMat = glm::transpose(glm::inverse(model));

    glUniformMatrix4fv(board.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
    glUniformMatrix4fv(board.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(board.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMat));

    glUniform3f(board.dirLightDirLocation, 0.5f, 1.0f, 0.3f);
    glUniform3f(board.dirLightColorLocation, 1.0f, 0.95f, 0.8f);

    glUniform3f(board.pointLightPosLocation, 0.0f, 3.0f, 0.0f);
    glUniform3f(board.pointLightColorLocation, 1.0f, 0.9f, 0.7f);
    glUniform1f(board.pointLightConstantLocation, 1.0f);
    glUniform1f(board.pointLightLinearLocation, 0.09f);
    glUniform1f(board.pointLightQuadraticLocation, 0.032f);

    glUniform3fv(board.spotLightPosLocation, 1, glm::value_ptr(camPos));
    glUniform3fv(board.spotLightDirLocation, 1, glm::value_ptr(camFront));
    glUniform3f(board.spotLightColorLocation, 1.0f, 1.0f, 1.0f);
    glUniform1f(board.spotLightCutoffLocation, glm::cos(glm::radians(12.5f)));
    glUniform1f(board.spotLightOuterCutoffLocation, glm::cos(glm::radians(17.5f)));

    glUniform1f(board.shininessLocation, 8.0f);
    glUniform3fv(board.cameraPosLocation, 1, glm::value_ptr(camPos));

    glBindVertexArray(board.vao);
    glDrawArrays(GL_TRIANGLES, 0, BOARD_VERTEX_COUNT);
    glBindVertexArray(0);
}