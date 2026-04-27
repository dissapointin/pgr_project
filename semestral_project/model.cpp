#include "model.h"
#include "camera.h"
#include "render.h"
#include <iostream>

extern bool spotLightOn;

static void processMesh(aiMesh* mesh, const aiScene* scene,
    const std::string& directory, Model& model) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        // position
        vertices.push_back(mesh->mVertices[i].x);
        vertices.push_back(mesh->mVertices[i].y);
        vertices.push_back(mesh->mVertices[i].z);
        // normal
        if (mesh->HasNormals()) {
            vertices.push_back(mesh->mNormals[i].x);
            vertices.push_back(mesh->mNormals[i].y);
            vertices.push_back(mesh->mNormals[i].z);
        }
        else {
            vertices.push_back(0); vertices.push_back(1); vertices.push_back(0);
        }
        // texcoord
        if (mesh->mTextureCoords[0]) {
            vertices.push_back(mesh->mTextureCoords[0][i].x);
            vertices.push_back(mesh->mTextureCoords[0][i].y);
        }
        else {
            vertices.push_back(0); vertices.push_back(0);
        }
    }

    // indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    Mesh m;
    m.indexCount = indices.size();
    m.texture = 0;

    // texture
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiString texPath;
        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
            std::string fullPath = directory + "/" + texPath.C_Str();
            m.texture = pgr::createTexture(fullPath.c_str());
        }
        float opacity = 1.0f;
        material->Get(AI_MATKEY_OPACITY, opacity);
        m.opacity = opacity;
    }

    // VAO + VBO + EBO
    glGenVertexArrays(1, &m.vao);
    glBindVertexArray(m.vao);

    glGenBuffers(1, &m.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
        vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW);

    int stride = 8 * sizeof(float);
    glEnableVertexAttribArray(model.posLocation);
    glVertexAttribPointer(model.posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(model.normalLocation);
    glVertexAttribPointer(model.normalLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(model.texCoordLocation);
    glVertexAttribPointer(model.texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
    model.meshes.push_back(m);
}

static void processNode(aiNode* node, const aiScene* scene,
    const std::string& directory, Model& model) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
        processMesh(scene->mMeshes[node->mMeshes[i]], scene, directory, model);
    for (unsigned int i = 0; i < node->mNumChildren; i++)
        processNode(node->mChildren[i], scene, directory, model);
}

void initModelShader(Model& model) {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/model.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/model.frag"),
        0
    };
    model.shaderProgram = pgr::createProgram(shaders);

    model.posLocation = glGetAttribLocation(model.shaderProgram, "position");
    model.normalLocation = glGetAttribLocation(model.shaderProgram, "normal");
    model.texCoordLocation = glGetAttribLocation(model.shaderProgram, "texCoord");
    model.PVMmatrixLocation = glGetUniformLocation(model.shaderProgram, "PVMmatrix");
    model.MmatrixLocation = glGetUniformLocation(model.shaderProgram, "Mmatrix");
    model.normalMatrixLocation = glGetUniformLocation(model.shaderProgram, "normalMatrix");
    model.shininessLocation = glGetUniformLocation(model.shaderProgram, "shininess");
    model.cameraPosLocation = glGetUniformLocation(model.shaderProgram, "cameraPos");
    model.dirLightDirLocation = glGetUniformLocation(model.shaderProgram, "dirLightDir");
    model.dirLightColorLocation = glGetUniformLocation(model.shaderProgram, "dirLightColor");
    model.pointLightPosLocation = glGetUniformLocation(model.shaderProgram, "pointLightPos");
    model.pointLightColorLocation = glGetUniformLocation(model.shaderProgram, "pointLightColor");
    model.pointLightConstantLocation = glGetUniformLocation(model.shaderProgram, "pointLightConstant");
    model.pointLightLinearLocation = glGetUniformLocation(model.shaderProgram, "pointLightLinear");
    model.pointLightQuadraticLocation = glGetUniformLocation(model.shaderProgram, "pointLightQuadratic");
    model.spotLightPosLocation = glGetUniformLocation(model.shaderProgram, "spotLightPos");
    model.spotLightDirLocation = glGetUniformLocation(model.shaderProgram, "spotLightDir");
    model.spotLightColorLocation = glGetUniformLocation(model.shaderProgram, "spotLightColor");
    model.spotLightCutoffLocation = glGetUniformLocation(model.shaderProgram, "spotLightCutoff");
    model.spotLightOuterCutoffLocation = glGetUniformLocation(model.shaderProgram, "spotLightOuterCutoff");
    model.texSamplerLocation = glGetUniformLocation(model.shaderProgram, "texSampler");
    model.hasTextureLocation = glGetUniformLocation(model.shaderProgram, "hasTexture");
    model.diffuseColorLocation = glGetUniformLocation(model.shaderProgram, "diffuseColor");
    model.opacityLocation = glGetUniformLocation(model.shaderProgram, "opacity");
}

bool loadModel(const std::string& path, Model& model) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path,
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Assimp error: " << importer.GetErrorString() << std::endl;
        return false;
    }

    std::string directory = path.substr(0, path.find_last_of('/'));
    processNode(scene->mRootNode, scene, directory, model);
    return true;
}

void drawModel(const Model& model, const glm::mat4& modelMatrix) {
    glUseProgram(model.shaderProgram);

    glm::vec3 camPos = getCameraPos();
    glm::vec3 camFront = getCameraFront();

    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * modelMatrix;
    glm::mat4 normalMat = glm::transpose(glm::inverse(modelMatrix));

    glUniformMatrix4fv(model.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
    glUniformMatrix4fv(model.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(model.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMat));

    glUniform3f(model.dirLightDirLocation, 0.5f, 1.0f, 0.3f);
    glUniform3f(model.dirLightColorLocation, 1.0f, 0.95f, 0.8f);

    glUniform3f(model.pointLightPosLocation, 0.0f, 3.0f, 0.0f);
    glUniform3f(model.pointLightColorLocation, 1.0f, 0.9f, 0.7f);
    glUniform1f(model.pointLightConstantLocation, 1.0f);
    glUniform1f(model.pointLightLinearLocation, 0.09f);
    glUniform1f(model.pointLightQuadraticLocation, 0.032f);

    glUniform3fv(model.spotLightPosLocation, 1, glm::value_ptr(camPos));
    glUniform3fv(model.spotLightDirLocation, 1, glm::value_ptr(camFront));
    glm::vec3 spotColor = spotLightOn ? glm::vec3(1.0f) : glm::vec3(0.0f);
    glUniform3fv(model.spotLightColorLocation, 1, glm::value_ptr(spotColor));
    glUniform1f(model.spotLightCutoffLocation, glm::cos(glm::radians(12.5f)));
    glUniform1f(model.spotLightOuterCutoffLocation, glm::cos(glm::radians(17.5f)));

    glUniform1f(model.shininessLocation, 32.0f);
    glUniform3fv(model.cameraPosLocation, 1, glm::value_ptr(camPos));
    glUniform1i(model.texSamplerLocation, 0);

    for (const Mesh& mesh : model.meshes) {
        if (mesh.texture != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.texture);
            glUniform1i(model.hasTextureLocation, 1);
        }
        else {
            glUniform1i(model.hasTextureLocation, 0);
            glUniform3f(model.diffuseColorLocation, 0.6f, 0.5f, 0.4f);
        }

        glUniform1f(model.opacityLocation, mesh.opacity);

        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}