//----------------------------------------------------------------------------------------
/**
 * \file    skybox.h
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Declaration of functions for the skybox.
 */
 //----------------------------------------------------------------------------------------

#pragma once
#include "pgr.h"

/// @brief Holds all OpenGL handles for the skybox cubemap
struct SkyboxGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;
    GLuint cubemapTexture;
    GLint posLocation;
    GLint viewMatrixLocation;
    GLint projMatrixLocation;
};

/// @brief Initialize skybox geometry and textures
void initSkybox();

/// @brief Draw skybox (call before other objects!)
void drawSkybox();