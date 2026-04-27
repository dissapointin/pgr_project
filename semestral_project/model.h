#pragma once
#include "pgr.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <string>

struct Mesh {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    GLuint texture;
    int indexCount;
	float opacity; // for transperancy, 1.0f = opaque, 0.0f = fully transparent
};

struct Model {
    std::vector<Mesh> meshes;
    GLuint shaderProgram;
    GLint posLocation;
    GLint normalLocation;
    GLint texCoordLocation;
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
void drawModel(const Model& model, const glm::mat4& modelMatrix);

/// @brief Initialize shader for model
/// @param model model to initialize shader for
void initModelShader(Model& model); 
