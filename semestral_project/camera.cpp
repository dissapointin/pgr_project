#include "camera.h"

// Camera position at the start
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.f; // horizontal angle
float pitch = 0.0f; // vertical angle
float speed = 0.1f; // speed of the camera
float sensitivity = 0.1f; // mouse sensitivity

// remember last mouse position for calculating movement between frames
int lastMouseX = 640;
int lastMouseY = 360;
bool firstMouse = true; // to not have jumps on the first mouse movement
bool freeCamera = true; // allow free camera movement

glm::mat4 projMatrix;

// Static camera positions
struct StaticCamera {
    glm::vec3 pos;
    glm::vec3 front;
};

StaticCamera staticCameras[2] = {
	{ glm::vec3(0.0f, 0.0f, 2.0f),  glm::vec3(0.0f, 0.0f, -1.0f) }, // front view
	{ glm::vec3(2.0f, 1.5f, 2.0f),  glm::vec3(-0.6f, -0.3f, -0.6f) } // angled view
};

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
    freeCamera = true;
}

void onKeyPress(unsigned char key) {
    if (!freeCamera) return;
    switch (key) {
    case 'w': cameraPos += speed * cameraFront; break;
    case 's': cameraPos -= speed * cameraFront; break;
    case 'a': cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed; break;
    case 'd': cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed; break;
    }
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