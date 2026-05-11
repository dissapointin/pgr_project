#pragma once
#include "pgr.h"

/// @brief Holds all OpenGL handles and shader locations for the blackboard geometry
struct BoardGeometry {
    GLuint vao;           // Vertex array object
    GLuint vbo;           // Vertex buffer object
    GLuint shaderProgram; // Compiled shader program

    // attribute locations
    GLint posLocation;
    GLint normalLocation;

    // uniform locations - matrices
    GLint PVMmatrixLocation;
    GLint MmatrixLocation;
    GLint normalMatrixLocation;

    // uniform locations - material
    GLint shininessLocation;
    GLint cameraPosLocation;

    // uniform locations - directional light
    GLint dirLightDirLocation;
    GLint dirLightColorLocation;

    // uniform locations - point light
    GLint pointLightPosLocation;
    GLint pointLightColorLocation;
    GLint pointLightConstantLocation; //Constant attenuation factor
    GLint pointLightLinearLocation; // Linear attenuation factor
    GLint pointLightQuadraticLocation; // Quadratic attenuation factor

    // uniform locations - spot light
    GLint spotLightPosLocation; // Position of spot light(camera)
    GLint spotLightDirLocation;
    GLint spotLightColorLocation;
    GLint spotLightCutoffLocation; // Inner cutoff angle (cos)
    GLint spotLightOuterCutoffLocation; // Outer cutoff angle (cos)
};

/// @brief Initialize blackboard geometry, shaders and GPU buffers
void initBoard();

/// @brief Draw blackboard on the back wall with Phong shading
void drawBoard();