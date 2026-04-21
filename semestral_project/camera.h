#pragma once
#include "pgr.h"

/// @brief Initialize camera to default position
void initCamera();

/// @brief Switch to one of the static camera views
/// @param index 0 or 1 - which static view
void setCameraStatic(int index);

/// @brief Switch to free camera mode
void setCameraFree();

/// @brief Handle keyboard input for camera movement (WASD)
/// @param key pressed key
void onKeyPress(unsigned char key);

/// @brief Handle special keys (arrows)
/// @param key special key code
void onSpecialKeyPress(int key);

/// @brief Handle mouse movement for camera rotation
/// @param x mouse x position
/// @param y mouse y position
void onMouseMove(int x, int y);

/// @brief Handle mouse click
/// @param button mouse button
/// @param state pressed or released
/// @param x mouse x position
/// @param y mouse y position
void onMouseClick(int button, int state, int x, int y);

/// @brief Update projection matrix on window resize
/// @param width new window width
/// @param height new window height
void updateProjection(int width, int height);

/// @brief Get current view matrix
/// @return view matrix
glm::mat4 getViewMatrix();

/// @brief Get current projection matrix
/// @return projection matrix
glm::mat4 getProjectionMatrix();

/// @brief Get current camera position
/// @return camera position
glm::vec3 getCameraPos();