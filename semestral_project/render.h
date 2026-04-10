#pragma once
#include "pgr.h"

/// @brief Initialize all scene objects (room, furniture, shaders)
void initScene();

/// @brief Draw the entire scene
void drawScene();

/// @brief Update scene animations (called every frame)
void updateScene();