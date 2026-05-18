//----------------------------------------------------------------------------------------
/**
 * \file    clock.h
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Declaration of functions for the clock textures.
 */
 //----------------------------------------------------------------------------------------

#pragma once
#include "pgr.h"

/// @brief Holds all OpenGL handles and shader locations for the clock
struct ClockGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;

    // Textures
    GLuint clockTexture;
    GLuint arrowTexture;

    // Positions
    GLint posLocation;
    GLint texCoordLocation;

    GLint PVMmatrixLocation;
    GLint texSamplerLocation;
    GLint useRotationLocation;
    GLint texMatrixLocation;
    GLint MmatrixLocation;
    GLint cameraPosLocation;
    GLint fogEnabledLocation;
    GLint fogStartLocation;
    GLint fogEndLocation;
    GLint fogColorLocation;
};

/// @brief Initialize clock geometry and textures
void initClock();

/// @brief Draw clock on the wall
void drawClock();
