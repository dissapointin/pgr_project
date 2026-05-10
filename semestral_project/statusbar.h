#pragma once
#include "pgr.h"

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