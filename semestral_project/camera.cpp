#include "camera.h"

void initCamera() {
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
}

glm::mat4 getViewMatrix() {
    return glm::mat4(1.0f);
}

glm::mat4 getProjectionMatrix() {
    return glm::mat4(1.0f);
}