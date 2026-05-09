#pragma once
#include "pgr.h"

struct FogTextureGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;
    GLuint texture;
    GLint posLocation;
    GLint texCoordLocation;
    GLint PVMmatrixLocation;
    GLint texSamplerLocation;
    GLint timeLocation;
};

/// @brief Initialize fog texture
void initFogTexture();

/// @brief Draw fog texture overlay
void drawFogTexture();