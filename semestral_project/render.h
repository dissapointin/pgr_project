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
    GLint shininessLocation;
    GLint cameraPosLocation;
    // dir light
    GLint dirLightDirLocation;
    GLint dirLightColorLocation;
    // point light
    GLint pointLightPosLocation;
    GLint pointLightColorLocation;
    GLint pointLightConstantLocation;
    GLint pointLightLinearLocation;
    GLint pointLightQuadraticLocation;
    // spot light
    GLint spotLightPosLocation;
    GLint spotLightDirLocation;
    GLint spotLightColorLocation;
    GLint spotLightCutoffLocation;
    GLint spotLightOuterCutoffLocation;
    // room textures
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
glm::vec3 getDirLightDir();
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