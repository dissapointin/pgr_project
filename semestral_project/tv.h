#pragma once
#include "pgr.h"

struct TVGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;
    GLuint spriteTexture;
    GLint posLocation;
    GLint texCoordLocation;
    GLint PVMmatrixLocation;
    GLint texSamplerLocation;
    GLint frameLocation;
    GLint colsLocation;
    GLint rowsLocation;
};

/// @brief Initialize TV screen geometry
void initTV();

/// @brief Draw animated TV screen
void drawTV();
