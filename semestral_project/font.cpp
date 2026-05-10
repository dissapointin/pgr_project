#include "font.h"
#include "render.h"
#include <sstream>
#include <iomanip>

FontGeometry fontObj;

const int FONT_COLS = 13;
const int FONT_ROWS = 7;

// map character to row/col in texture
static bool charToIndex(char c, int& col, int& row) {
    if (c >= 'A' && c <= 'M') { row = 0; col = c - 'A'; return true; }
    if (c >= 'N' && c <= 'Z') { row = 1; col = c - 'N'; return true; }
    if (c >= 'a' && c <= 'm') { row = 2; col = c - 'a'; return true; }
    if (c >= 'n' && c <= 'z') { row = 3; col = c - 'n'; return true; }
    if (c >= '0' && c <= '9') { row = 4; col = c - '0'; return true; }
    if (c == '+') { row = 4; col = 10; return true; }
    if (c == '-') { row = 4; col = 11; return true; }
    if (c == '=') { row = 4; col = 12; return true; }
    if (c == '(') { row = 5; col = 0; return true; }
    if (c == ')') { row = 5; col = 1; return true; }
    if (c == ':') { row = 5; col = 9; return true; }
    if (c == '.') { row = 6; col = 2; return true; }
    if (c == '!') { row = 6; col = 0; return true; }
    if (c == '?') { row = 6; col = 1; return true; }
    return false;
}

static const float charQuad[] = {
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
};

void initFont() {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/font.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/font.frag"),
        0
    };
    fontObj.shaderProgram = pgr::createProgram(shaders);

    fontObj.posLocation = glGetAttribLocation(fontObj.shaderProgram, "position");
    fontObj.texCoordLocation = glGetAttribLocation(fontObj.shaderProgram, "texCoord");
    fontObj.PVMmatrixLocation = glGetUniformLocation(fontObj.shaderProgram, "PVMmatrix");
    fontObj.texSamplerLocation = glGetUniformLocation(fontObj.shaderProgram, "texSampler");
    fontObj.charIndexLocation = glGetUniformLocation(fontObj.shaderProgram, "charIndex");

    glGenVertexArrays(1, &fontObj.vao);
    glBindVertexArray(fontObj.vao);

    glGenBuffers(1, &fontObj.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, fontObj.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(charQuad), charQuad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(fontObj.posLocation);
    glVertexAttribPointer(fontObj.posLocation, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(fontObj.texCoordLocation);
    glVertexAttribPointer(fontObj.texCoordLocation, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    fontObj.texture = pgr::createTexture("textures/font.png");
}

void drawText(const std::string& text, float x, float y, float scale) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(fontObj.shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fontObj.texture);
    glUniform1i(fontObj.texSamplerLocation, 0);
    glBindVertexArray(fontObj.vao);

    float charW = scale * 2.0f / 13.0f;
    float charH = scale * 2.0f / 7.0f;
    float curX = x;

    for (char c : text) {
        if (c == ' ') { curX += charW; continue; }

        int col, row;
        if (!charToIndex(c, col, row)) { curX += charW; continue; }

        // build orthographic matrix for this character
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(curX, y, 0.0f));
        model = glm::scale(model, glm::vec3(charW, charH, 1.0f));

        glUniformMatrix4fv(fontObj.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniform2i(fontObj.charIndexLocation, col, row);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        curX += charW;
    }

    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}