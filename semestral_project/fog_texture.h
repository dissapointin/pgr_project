//----------------------------------------------------------------------------------------
/**
 * \file    fog_texture.h
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Declaration of function for the fog texture.
 */
 //----------------------------------------------------------------------------------------

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