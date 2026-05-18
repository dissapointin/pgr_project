//----------------------------------------------------------------------------------------
/**
 * \file    obj_loader.h
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Declaration of functions for loading OBJ models.
 */
 //----------------------------------------------------------------------------------------

#pragma once
#include "pgr.h"
#include <string>
#include <vector>

/// @brief Single mesh loaded from OBJ file without external library
struct ObjMesh {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    int indexCount;
    glm::vec3 diffuseColor;
    GLuint shaderProgram;

    // attribute locations
    GLint posLocation;
    GLint normalLocation;
    GLint texCoordLocation;

    // matrix uniforms
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

/// @brief Load OBJ file without external library
/// @param path path to .obj file
/// @param mesh output mesh
/// @return true if successful
bool loadOBJ(const std::string& path, ObjMesh& mesh);

/// @brief Initialize shader for OBJ mesh
/// @param mesh mesh to initialize
void initObjShader(ObjMesh& mesh);

/// @brief Draw OBJ mesh
/// @param mesh mesh to draw
/// @param modelMatrix transformation matrix
/// @param shininess shininess value
void drawObjMesh(const ObjMesh& mesh, const glm::mat4& modelMatrix, float shininess = 32.0f);
