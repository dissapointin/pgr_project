#pragma once
#include "pgr.h"

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