#include "obj_loader.h"
#include "camera.h"
#include "render.h"
#include <fstream>
#include <sstream>
#include <iostream>

extern bool spotLightOn;
extern bool fogEnabled;

static glm::vec3 loadMTL(const std::string& mtlPath) {
    std::ifstream file(mtlPath);
    if (!file.is_open()) return glm::vec3(0.6f, 0.5f, 0.4f);

    glm::vec3 color(0.6f, 0.5f, 0.4f);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        ss >> token;
        if (token == "Kd") {
            ss >> color.r >> color.g >> color.b;
        }
    }
    return color;
}

bool loadOBJ(const std::string& path, ObjMesh& mesh) {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;

    std::vector<float>        vertices;
    std::vector<unsigned int> indices;

    std::string mtlFile = "";

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Cannot open OBJ file: " << path << std::endl;
        return false;
    }

    std::vector<glm::ivec3> faceIndices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string token;
        ss >> token;

        if (token == "v") {
            glm::vec3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (token == "vn") {
            glm::vec3 norm;
            ss >> norm.x >> norm.y >> norm.z;
            normals.push_back(norm);
        }
        else if (token == "vt") {
            glm::vec2 tc;
            ss >> tc.x >> tc.y;
            texCoords.push_back(tc);
        }
        else if (token == "mtllib") {
            ss >> mtlFile;
        }
        else if (token == "f") {
            std::vector<glm::ivec3> faceVerts;
            std::string vertStr;
            while (ss >> vertStr) {
                glm::ivec3 idx(0, 0, 0);
                std::replace(vertStr.begin(), vertStr.end(), '/', ' ');
                std::istringstream vs(vertStr);
                vs >> idx.x;
                if (vs.peek() != EOF) vs >> idx.y;
                if (vs.peek() != EOF) vs >> idx.z;
                faceVerts.push_back(idx);
            }
            for (int i = 1; i + 1 < (int)faceVerts.size(); i++) {
                faceIndices.push_back(faceVerts[0]);
                faceIndices.push_back(faceVerts[i]);
                faceIndices.push_back(faceVerts[i + 1]);
            }
        }
    }
    file.close();

    // load material color
    if (!mtlFile.empty()) {
        std::string dir = path.substr(0, path.find_last_of('/'));
        mesh.diffuseColor = loadMTL(dir + "/" + mtlFile);
    }
    else {
        mesh.diffuseColor = glm::vec3(0.6f, 0.5f, 0.4f);
    }

    // build flat vertex array
    for (unsigned int i = 0; i < faceIndices.size(); i++) {
        glm::ivec3 idx = faceIndices[i];
        int vi = idx.x - 1;
        int vti = idx.y - 1;
        int vni = idx.z - 1;

        glm::vec3 pos = (vi >= 0 && vi < (int)positions.size()) ? positions[vi] : glm::vec3(0);
        glm::vec3 norm = (vni >= 0 && vni < (int)normals.size()) ? normals[vni] : glm::vec3(0, 1, 0);
        glm::vec2 tc = (vti >= 0 && vti < (int)texCoords.size()) ? texCoords[vti] : glm::vec2(0);

        vertices.push_back(pos.x);  vertices.push_back(pos.y);  vertices.push_back(pos.z);
        vertices.push_back(norm.x); vertices.push_back(norm.y); vertices.push_back(norm.z);
        vertices.push_back(tc.x);   vertices.push_back(tc.y);

        indices.push_back(i);
    }

    mesh.indexCount = (int)indices.size();

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
        vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW);

    int stride = 8 * sizeof(float);
    glEnableVertexAttribArray(mesh.posLocation);
    glVertexAttribPointer(mesh.posLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(mesh.normalLocation);
    glVertexAttribPointer(mesh.normalLocation, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(mesh.texCoordLocation);
    glVertexAttribPointer(mesh.texCoordLocation, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

    glBindVertexArray(0);
    return true;
}

void initObjShader(ObjMesh& mesh) {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/model.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/model.frag"),
        0
    };
    mesh.shaderProgram = pgr::createProgram(shaders);

    mesh.posLocation = glGetAttribLocation(mesh.shaderProgram, "position");
    mesh.normalLocation = glGetAttribLocation(mesh.shaderProgram, "normal");
    mesh.texCoordLocation = glGetAttribLocation(mesh.shaderProgram, "texCoord");
    mesh.PVMmatrixLocation = glGetUniformLocation(mesh.shaderProgram, "PVMmatrix");
    mesh.MmatrixLocation = glGetUniformLocation(mesh.shaderProgram, "Mmatrix");
    mesh.normalMatrixLocation = glGetUniformLocation(mesh.shaderProgram, "normalMatrix");
    mesh.shininessLocation = glGetUniformLocation(mesh.shaderProgram, "shininess");
    mesh.cameraPosLocation = glGetUniformLocation(mesh.shaderProgram, "cameraPos");
    mesh.dirLightDirLocation = glGetUniformLocation(mesh.shaderProgram, "dirLightDir");
    mesh.dirLightColorLocation = glGetUniformLocation(mesh.shaderProgram, "dirLightColor");
    mesh.pointLightPosLocation = glGetUniformLocation(mesh.shaderProgram, "pointLightPos");
    mesh.pointLightColorLocation = glGetUniformLocation(mesh.shaderProgram, "pointLightColor");
    mesh.pointLightConstantLocation = glGetUniformLocation(mesh.shaderProgram, "pointLightConstant");
    mesh.pointLightLinearLocation = glGetUniformLocation(mesh.shaderProgram, "pointLightLinear");
    mesh.pointLightQuadraticLocation = glGetUniformLocation(mesh.shaderProgram, "pointLightQuadratic");
    mesh.spotLightPosLocation = glGetUniformLocation(mesh.shaderProgram, "spotLightPos");
    mesh.spotLightDirLocation = glGetUniformLocation(mesh.shaderProgram, "spotLightDir");
    mesh.spotLightColorLocation = glGetUniformLocation(mesh.shaderProgram, "spotLightColor");
    mesh.spotLightCutoffLocation = glGetUniformLocation(mesh.shaderProgram, "spotLightCutoff");
    mesh.spotLightOuterCutoffLocation = glGetUniformLocation(mesh.shaderProgram, "spotLightOuterCutoff");
    mesh.diffuseColorLocation = glGetUniformLocation(mesh.shaderProgram, "diffuseColor");
    mesh.fogEnabledLocation = glGetUniformLocation(mesh.shaderProgram, "fogEnabled");
    mesh.fogStartLocation = glGetUniformLocation(mesh.shaderProgram, "fogStart");
    mesh.fogEndLocation = glGetUniformLocation(mesh.shaderProgram, "fogEnd");
    mesh.fogColorLocation = glGetUniformLocation(mesh.shaderProgram, "fogColor");
}

void drawObjMesh(const ObjMesh& mesh, const glm::mat4& modelMatrix, float shininess) {
    glUseProgram(mesh.shaderProgram);

    glm::vec3 camPos = getCameraPos();
    glm::vec3 camFront = getCameraFront();

    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * modelMatrix;
    glm::mat4 normalMat = glm::transpose(glm::inverse(modelMatrix));

    glm::vec3 pointColor = pointLightOn ? glm::vec3(1.0f, 0.9f, 0.7f) : glm::vec3(0.0f);

    glUniformMatrix4fv(mesh.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
    glUniformMatrix4fv(mesh.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(mesh.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMat));

    glUniform3f(mesh.dirLightDirLocation, 0.5f, 1.0f, 0.3f);
    glUniform3f(mesh.dirLightColorLocation, 1.0f, 0.95f, 0.8f);

    glUniform3f(mesh.pointLightPosLocation, 0.0f, 3.0f, 0.0f);
    glUniform3fv(mesh.pointLightColorLocation, 1, glm::value_ptr(pointColor));
    glUniform1f(mesh.pointLightConstantLocation, 1.0f);
    glUniform1f(mesh.pointLightLinearLocation, 0.09f);
    glUniform1f(mesh.pointLightQuadraticLocation, 0.032f);

    glUniform3fv(mesh.spotLightPosLocation, 1, glm::value_ptr(camPos));
    glUniform3fv(mesh.spotLightDirLocation, 1, glm::value_ptr(camFront));
    glm::vec3 spotColor = spotLightOn ? glm::vec3(1.0f) : glm::vec3(0.0f);
    glUniform3fv(mesh.spotLightColorLocation, 1, glm::value_ptr(spotColor));
    glUniform1f(mesh.spotLightCutoffLocation, glm::cos(glm::radians(12.5f)));
    glUniform1f(mesh.spotLightOuterCutoffLocation, glm::cos(glm::radians(17.5f)));

    glUniform1f(mesh.shininessLocation, shininess);
    glUniform3fv(mesh.cameraPosLocation, 1, glm::value_ptr(camPos));
    glUniform3fv(mesh.diffuseColorLocation, 1, glm::value_ptr(mesh.diffuseColor));

    glUniform1i(mesh.fogEnabledLocation, fogEnabled ? 1 : 0);
    glUniform1f(mesh.fogStartLocation, 1.0f);
    glUniform1f(mesh.fogEndLocation, 10.0f);
    glUniform3f(mesh.fogColorLocation, 0.7f, 0.7f, 0.7f);

    // no texture for OBJ loaded objects
    glUniform1i(glGetUniformLocation(mesh.shaderProgram, "hasTexture"), 0);
    glUniform1f(glGetUniformLocation(mesh.shaderProgram, "opacity"), 1.0f);
    glUniform1f(glGetUniformLocation(mesh.shaderProgram, "specularStrength"), 0.05f);

    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}