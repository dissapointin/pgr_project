#include "camera.h"

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 4.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

glm::mat4 projMatrix;

void initCamera() {
    projMatrix = glm::perspective(
        glm::radians(60.0f),
        1280.0f / 720.0f,
        0.1f, 100.0f
    );
}

void setCameraStatic(int index) {
}

void setCameraFree() {
}

void onKeyPress(unsigned char key) {
}

void onSpecialKeyPress(int key) {
}

void onMouseMove(int x, int y) {
}

void onMouseClick(int button, int state, int x, int y) {
}

void updateProjection(int width, int height) {
    projMatrix = glm::perspective(
        glm::radians(60.0f),
        (float)width / (float)height,
        0.1f, 100.0f
    );
}

glm::mat4 getViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 getProjectionMatrix() {
    return projMatrix;
}