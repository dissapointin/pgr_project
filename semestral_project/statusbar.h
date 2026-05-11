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