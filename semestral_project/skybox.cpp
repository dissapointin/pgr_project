#include "skybox.h"
#include "camera.h"
#include <iostream>

SkyboxGeometry skybox;

static const float skyboxVertices[] = {
    -1, 1,-1,  -1,-1,-1,   1,-1,-1,   1,-1,-1,   1, 1,-1,  -1, 1,-1,
    -1,-1, 1,  -1,-1,-1,  -1, 1,-1,  -1, 1,-1,  -1, 1, 1,  -1,-1, 1,
     1,-1,-1,   1,-1, 1,   1, 1, 1,   1, 1, 1,   1, 1,-1,   1,-1,-1,
    -1,-1, 1,  -1, 1, 1,   1, 1, 1,   1, 1, 1,   1,-1, 1,  -1,-1, 1,
    -1, 1,-1,   1, 1,-1,   1, 1, 1,   1, 1, 1,  -1, 1, 1,  -1, 1,-1,
    -1,-1,-1,  -1,-1, 1,   1,-1,-1,   1,-1,-1,  -1,-1, 1,   1,-1, 1
};

void initSkybox() {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/skybox.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/skybox.frag"),
        0
    };
    skybox.shaderProgram = pgr::createProgram(shaders);

    skybox.posLocation = glGetAttribLocation(skybox.shaderProgram, "position");
    skybox.viewMatrixLocation = glGetUniformLocation(skybox.shaderProgram, "viewMatrix");
    skybox.projMatrixLocation = glGetUniformLocation(skybox.shaderProgram, "projMatrix");

    glGenVertexArrays(1, &skybox.vao);
    glBindVertexArray(skybox.vao);

    glGenBuffers(1, &skybox.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, skybox.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(skybox.posLocation);
    glVertexAttribPointer(skybox.posLocation, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0);

    glBindVertexArray(0);

    // load cubemap
    glGenTextures(1, &skybox.cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemapTexture);

    std::vector<std::string> faces = {
        "skybox/posx.jpg", "skybox/negx.jpg",
        "skybox/negy.jpg", "skybox/posy.jpg",
        "skybox/posz.jpg", "skybox/negz.jpg"
    };

    for (unsigned int i = 0; i < faces.size(); i++) {
        pgr::loadTexImage2D(faces[i].c_str(), GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void drawSkybox() {
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_FALSE);
    glUseProgram(skybox.shaderProgram);
    glUniform1i(glGetUniformLocation(skybox.shaderProgram, "skybox"), 0);

    // remove translation from view matrix
    glm::mat4 view = glm::mat4(glm::mat3(getViewMatrix()));
    glm::mat4 proj = getProjectionMatrix();

    glUniformMatrix4fv(skybox.viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(skybox.projMatrixLocation, 1, GL_FALSE, glm::value_ptr(proj));

    glBindVertexArray(skybox.vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}