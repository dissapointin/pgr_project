#pragma once
#include "pgr.h"
#include <string>

/// @brief Holds all OpenGL handles for bitmap font rendering
struct FontGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;
    GLuint texture;
    GLint posLocation;
    GLint texCoordLocation;
    GLint PVMmatrixLocation;
    GLint texSamplerLocation;
    GLint charIndexLocation;
};

/// @brief Initialize font
void initFont();

/// @brief Draw string on screen
/// @param text text to draw
/// @param x screen x position (-1 to 1)
/// @param y screen y position (-1 to 1)
/// @param scale character scale
void drawText(const std::string& text, float x, float y, float scale);