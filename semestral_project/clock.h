#pragma once
#include "pgr.h"

struct ClockGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;
    GLuint clockTexture;
    GLuint arrowTexture;
    GLint posLocation;
    GLint texCoordLocation;
    GLint PVMmatrixLocation;
    GLint texSamplerLocation;
    GLint rotationAngleLocation;
    GLint useRotationLocation;
};

/// @brief Initialize clock geometry and textures
void initClock();

/// @brief Draw clock on the wall
void drawClock();
