#include "statusbar.h"
#include "render.h"

StatusBarGeometry sb;

// full screen quad in NDC: bottom strip
static const float sbVertices[] = {
    // x      y      z
    -0.9f, -0.95f, 0.0f,
     0.9f, -0.95f, 0.0f,
     0.9f, -0.85f, 0.0f,
    -0.9f, -0.95f, 0.0f,
     0.9f, -0.85f, 0.0f,
    -0.9f, -0.85f, 0.0f,
};

void initStatusBar() {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/statusbar.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/statusbar.frag"),
        0
    };
    sb.shaderProgram = pgr::createProgram(shaders);

    sb.posLocation = glGetAttribLocation(sb.shaderProgram, "position");
    sb.colorLocation = glGetUniformLocation(sb.shaderProgram, "barColor");
    sb.fillLocation = glGetUniformLocation(sb.shaderProgram, "fillAmount");

    glGenVertexArrays(1, &sb.vao);
    glBindVertexArray(sb.vao);

    glGenBuffers(1, &sb.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, sb.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sbVertices), sbVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(sb.posLocation);
    glVertexAttribPointer(sb.posLocation, 3, GL_FLOAT, GL_FALSE,
        3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}

void drawStatusBar() {
    glDisable(GL_DEPTH_TEST);

    glUseProgram(sb.shaderProgram);

    // color based on timeOfDay
    glm::vec3 color;
    if (timeOfDay < 0.25f)
        color = glm::vec3(1.0f, 0.6f, 0.3f);  // morning
    else if (timeOfDay < 0.5f)
        color = glm::vec3(1.0f, 0.95f, 0.5f); // day
    else if (timeOfDay < 0.75f)
        color = glm::vec3(1.0f, 0.4f, 0.1f);  // evening
    else
        color = glm::vec3(0.1f, 0.1f, 0.4f);  // night

    glUniform3fv(sb.colorLocation, 1, glm::value_ptr(color));
    glUniform1f(sb.fillLocation, timeOfDay);

    glBindVertexArray(sb.vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}