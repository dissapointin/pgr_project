//----------------------------------------------------------------------------------------
/**
 * \file    statusbar.h
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Declaration of functions for the status bar.
 */
 //----------------------------------------------------------------------------------------

#pragma once
#include "pgr.h"

/// @brief Holds all OpenGL handles for the status bar HUD element
struct StatusBarGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;
    GLint posLocation;
    GLint colorLocation;
    GLint fillLocation;
};

/// @brief Initialize status bar
void initStatusBar();

/// @brief Draw status bar
void drawStatusBar();