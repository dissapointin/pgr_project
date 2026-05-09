#include "fog_texture.h"
#include "camera.h"
#include "render.h"

FogTextureGeometry fogTex;

static const float fogQuadVertices[] = {
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
};

void initFogTexture() {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/fog_texture.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/fog_texture.frag"),
        0
    };
    fogTex.shaderProgram = pgr::createProgram(shaders);

    fogTex.posLocation = glGetAttribLocation(fogTex.shaderProgram, "position");
    fogTex.texCoordLocation = glGetAttribLocation(fogTex.shaderProgram, "texCoord");
    fogTex.PVMmatrixLocation = glGetUniformLocation(fogTex.shaderProgram, "PVMmatrix");
    fogTex.texSamplerLocation = glGetUniformLocation(fogTex.shaderProgram, "texSampler");
    fogTex.timeLocation = glGetUniformLocation(fogTex.shaderProgram, "time");

    glGenVertexArrays(1, &fogTex.vao);
    glBindVertexArray(fogTex.vao);

    glGenBuffers(1, &fogTex.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, fogTex.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(fogQuadVertices), fogQuadVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(fogTex.posLocation);
    glVertexAttribPointer(fogTex.posLocation, 3, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(fogTex.texCoordLocation);
    glVertexAttribPointer(fogTex.texCoordLocation, 2, GL_FLOAT, GL_FALSE,
        5 * sizeof(float), (void*)(3 * sizeof(float)));

    glBindVertexArray(0);

    fogTex.texture = pgr::createTexture("textures/fog_texture.png");
}

void drawFogTexture() {
    if (!fogEnabled) return;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    glUseProgram(fogTex.shaderProgram);

    // quad always in front of camera
    glm::vec3 camPos = getCameraPos();
    glm::vec3 camFront = getCameraFront();
    glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camRight = glm::normalize(glm::cross(camFront, camUp));

    glm::mat4 model = glm::mat4(1.0f);
    model[0] = glm::vec4(camRight * 20.0f, 0.0f);
    model[1] = glm::vec4(camUp * 20.0f, 0.0f);
    model[2] = glm::vec4(camFront, 0.0f);
    model[3] = glm::vec4(camPos + camFront * 2.0f, 1.0f);

    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * model;
    glUniformMatrix4fv(fogTex.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));

    float timeMs = (float)glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    glUniform1f(fogTex.timeLocation, timeMs);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fogTex.texture);
    glUniform1i(fogTex.texSamplerLocation, 0);

    glBindVertexArray(fogTex.vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}