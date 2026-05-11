#pragma once
#include "pgr.h"

/// @brief Fan movement mode
enum FanMode {
    FAN_STOPPED = 0,
    FAN_CIRCLE,      // a) circle
    FAN_CATMULL_ROM, // b) Catmull-Rom
    FAN_BEZIER       // c) Bezier
};

/// @brief Holds all OpenGL handles for the procedurally generated fan
struct FanGeometry {
    // base (level 1)
    GLuint baseVao, baseVbo;
    int baseVertexCount;
    // head (level 2)
    GLuint headVao, headVbo;
    int headVertexCount;
    // blades (level 3)
    GLuint bladeVao, bladeVbo;
    int bladeVertexCount;

    GLuint shaderProgram;
    GLint posLocation;
    GLint normalLocation;
    GLint PVMmatrixLocation;
    GLint MmatrixLocation;
    GLint normalMatrixLocation;
    GLint shininessLocation;
    GLint cameraPosLocation;
    GLint dirLightDirLocation;
    GLint dirLightColorLocation;
    GLint pointLightPosLocation;
    GLint pointLightColorLocation;
    GLint pointLightConstantLocation;
    GLint pointLightLinearLocation;
    GLint pointLightQuadraticLocation;
    GLint spotLightPosLocation;
    GLint spotLightDirLocation;
    GLint spotLightColorLocation;
    GLint spotLightCutoffLocation;
    GLint spotLightOuterCutoffLocation;
    GLint diffuseColorLocation;
    GLint fogEnabledLocation;
    GLint fogStartLocation;
    GLint fogEndLocation;
    GLint fogColorLocation;
};

extern FanMode fanMode; // Current fan movement mode
extern float fanHeadAngle; // Current head oscillation angle in degrees
extern float fanBladeAngle; // Current blade rotation angle in degrees
extern glm::vec3 fanPosition; // Current fan position in world space
extern glm::vec3 catmullPoints[]; // Control points for Catmull-Rom curve, modifiable via config

/// @brief Initialize fan geometry and shaders
void initFan();

/// @brief Draw fan at current position with hierarchy
void drawFan();

/// @brief Update fan animation
void updateFan();