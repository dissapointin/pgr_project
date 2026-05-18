//----------------------------------------------------------------------------------------
/**
 * \file    model.h
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Declaration of functions for the model loading and rendering.
 */
 //----------------------------------------------------------------------------------------

#pragma once
#include "pgr.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

/// @brief Single mesh within a model - holds GPU buffers and material properties
struct Mesh {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint texture;
    int indexCount;
	float opacity; // for transperancy, 1.0f = opaque, 0.0f = fully transparent
    glm::vec3 diffuseColor;
};

/// @brief 3D model loaded via Assimp - contains multiple meshes
struct Model {
    std::vector<Mesh> meshes;
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
    GLint texSamplerLocation;
    GLint hasTextureLocation;
    GLint diffuseColorLocation;
    GLint opacityLocation;
};

/// @brief Load model from .obj file
/// @param path path to .obj file
/// @param model output model struct
/// @return true if successful
bool loadModel(const std::string& path, Model& model);

/// @brief Draw model at given position
/// @param model model to draw
/// @param modelMatrix model transformation matrix
void drawModel(const Model& model, const glm::mat4& modelMatrix,
    float shininess = 32.0f);

/// @brief Initialize shader for model
/// @param model model to initialize shader for
void initModelShader(Model& model); 
