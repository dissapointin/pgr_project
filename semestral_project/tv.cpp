#include "tv.h"
#include "camera.h"

TVGeometry tv;

const int TV_COLS = 8;
const int TV_ROWS = 30;
const int TV_TOTAL_FRAMES = TV_COLS * TV_ROWS;
const float TV_FPS = 24.0f;

static const float tvQuadVertices[] = {
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
};

void initTV() {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/tv.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/tv.frag"),
        0
    };
    tv.shaderProgram = pgr::createProgram(shaders);

    tv.posLocation = glGetAttribLocation(tv.shaderProgram, "position");
    tv.texCoordLocation = glGetAttribLocation(tv.shaderProgram, "texCoord");
    tv.PVMmatrixLocation = glGetUniformLocation(tv.shaderProgram, "PVMmatrix");
    tv.texSamplerLocation = glGetUniformLocation(tv.shaderProgram, "texSampler");
    tv.frameLocation = glGetUniformLocation(tv.shaderProgram, "currentFrame");
    tv.colsLocation = glGetUniformLocation(tv.shaderProgram, "cols");
    tv.rowsLocation = glGetUniformLocation(tv.shaderProgram, "rows");

    glGenVertexArrays(1, &tv.vao);
    glBindVertexArray(tv.vao);

    glGenBuffers(1, &tv.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, tv.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tvQuadVertices), tvQuadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(tv.posLocation);
    glVertexAttribPointer(tv.posLocation, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(tv.texCoordLocation);
    glVertexAttribPointer(tv.texCoordLocation, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    tv.spriteTexture = pgr::createTexture("textures/tv_sprite_sheet.png");
}

void drawTV() {
    glUseProgram(tv.shaderProgram);

    // position on teacher's desk, facing camera
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, -0.77f, -5.55f));
    model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.0f));
    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * model;
    glUniformMatrix4fv(tv.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

    // calculate current frame
    float timeMs = (float)glutGet(GLUT_ELAPSED_TIME);
    int currentFrame = (int)(timeMs / 1000.0f * TV_FPS) % TV_TOTAL_FRAMES;

    glUniform1i(tv.frameLocation, currentFrame);
    glUniform1i(tv.colsLocation, TV_COLS);
    glUniform1i(tv.rowsLocation, TV_ROWS);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tv.spriteTexture);
    glUniform1i(tv.texSamplerLocation, 0);

    glBindVertexArray(tv.vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}