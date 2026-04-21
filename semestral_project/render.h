#pragma once
#include "pgr.h"

// Room geometry buffers
struct RoomGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;
    GLint posLocation;
    GLint normalLocation;
    GLint PVMmatrixLocation;
    GLint MmatrixLocation;
    GLint normalMatrixLocation;
    GLint lightDirLocation;
    GLint lightColorLocation;
    GLint shininessLocation;
    GLint cameraPosLocation;
};

/// @brief Initialize all scene objects (room, furniture, shaders)
void initScene();

/// @brief Draw the entire scene
void drawScene();

/// @brief Update scene animations (called every frame)
void updateScene();