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
    GLint MmatrixLocation;
    GLint cameraPosLocation;
    GLint fogEnabledLocation;
    GLint fogStartLocation;
    GLint fogEndLocation;
    GLint fogColorLocation;
};

/// @brief Initialize TV screen geometry
void initTV();

/// @brief Draw animated TV screen
void drawTV();
