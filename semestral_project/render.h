#pragma once
#include "pgr.h"

/// @brief Holds all OpenGL handles for the room geometry
struct RoomGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;
    GLint posLocation;
    GLint normalLocation;
    GLint PVMmatrixLocation;
    GLint MmatrixLocation;
    GLint normalMatrixLocation;
    GLint shininessLocation;
    GLint cameraPosLocation;
    // directional light
    GLint dirLightDirLocation;
    GLint dirLightColorLocation;
    // point light (ceiling lamp)
    GLint pointLightPosLocation;
    GLint pointLightColorLocation;
    GLint pointLightConstantLocation;
    GLint pointLightLinearLocation;
    GLint pointLightQuadraticLocation;
    // spot light (flashlight)
    GLint spotLightPosLocation;
    GLint spotLightDirLocation;
    GLint spotLightColorLocation;
    GLint spotLightCutoffLocation;
    GLint spotLightOuterCutoffLocation;
    // textures
    GLuint floorTexture;
    GLuint wallTexture;
    GLuint ceilingTexture;
    GLint texSamplerLocation;
    GLint wallSamplerLocation;
    GLint ceilingSamplerLocation;
};

// for config
extern float configFogStart;
extern float configFogEnd;

extern float timeOfDay;
extern bool autoTime;

/// @brief Get directional light direction based on current timeOfDay
/// @return normalized direction vector
glm::vec3 getDirLightDir();

/// @brief Get directional light color based on current timeOfDay
/// @return RGB color vector
glm::vec3 getDirLightColor();

// For light picking
extern bool pointLightOn;

// For succulent animation
extern glm::vec3 succulentPos;
extern bool succulentJumping;
extern float succulentJumpVelocity;
extern float succulentBaseY;

extern bool spotLightOn;

extern bool encyclopediaPicked;
extern glm::vec3 encyclopediaPos;
extern glm::vec3 encyclopediaDropPos;
extern bool encyclopediaFalling;
extern float encyclopediaVelocity;

extern bool fogEnabled;

/// @brief Initialize all scene objects (room, furniture, shaders)
void initScene();

/// @brief Draw the entire scene
void drawScene();

/// @brief Update scene animations (called every frame)
void updateScene();