#pragma once
#include "pgr.h"

/// @brief Holds all OpenGL handles for the fog texture overlay
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