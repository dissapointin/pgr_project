//----------------------------------------------------------------------------------------
/**
 * \file    camera.cpp
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Implementation of functions for the camera.
 */
 //----------------------------------------------------------------------------------------

#include "camera.h"
#include "render.h"
#include <iostream>

bool zPressed = false;

// Camera position at the start
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float yaw = -90.f; // Horizontal rotation angle in degrees
float pitch = 0.0f; // Vertical rotation angle in degrees
float speed = 0.1f; // Camera movement speed per frame
float sensitivity = 0.3f; // Mouse look sensitivity

// remember last mouse position for calculating movement between frames
int lastMouseX = 640; // Last mouse X position for delta calculation
int lastMouseY = 360; // Last mouse Y position for delta calculation
bool firstMouse = true; // True until first mouse movement to prevent camera jump
bool freeCamera = true; // True when free camera mode is active

glm::mat4 projMatrix;

/// @brief Stores position and look direction for a static camera view
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
    freeCamera = false;
    cameraPos = staticCameras[index].pos;
    cameraFront = glm::normalize(staticCameras[index].front);
}

void setCameraFree() {
    freeCamera = true;
}

void onKeyPress(unsigned char key) {
    if (!freeCamera) return;
    glm::vec3 flatFront = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    glm::vec3 oldPos = cameraPos;
    switch (key) {
    case 'w': 
        cameraPos += speed * flatFront;
        checkCollision(cameraPos, oldPos);
        break;
    case 's': 
        cameraPos -= speed * flatFront; 
        checkCollision(cameraPos, oldPos);
        break;
    case 'a': 
        cameraPos -= glm::normalize(glm::cross(flatFront, cameraUp)) * speed; 
        checkCollision(cameraPos, oldPos);
        break;
    case 'd': 
        cameraPos += glm::normalize(glm::cross(flatFront, cameraUp)) * speed; 
        checkCollision(cameraPos, oldPos);
        break;
    }
}

void onSpecialKeyPress(int key) {
    if (!freeCamera) return;
    glm::vec3 flatFront = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
    glm::vec3 oldPos = cameraPos;
    switch (key) {
    case GLUT_KEY_UP:
        cameraPos += speed * flatFront; 
        checkCollision(cameraPos, oldPos);
        break;
    case GLUT_KEY_DOWN:  
        cameraPos -= speed * flatFront; 
        checkCollision(cameraPos, oldPos);
        break;
    case GLUT_KEY_LEFT:  
        cameraPos -= glm::normalize(glm::cross(flatFront, cameraUp)) * speed; 
        checkCollision(cameraPos, oldPos);
        break;
    case GLUT_KEY_RIGHT: 
        cameraPos += glm::normalize(glm::cross(flatFront, cameraUp)) * speed; 
        checkCollision(cameraPos, oldPos);
        break;
    case GLUT_KEY_F1: collisionEnabled = false; break;
    case GLUT_KEY_F2: collisionEnabled = true;  break;
    }
}

void onMouseMove(int x, int y) {
    if (!freeCamera) return;

    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }

    float dx = (x - lastMouseX) * sensitivity;
    float dy = (lastMouseY - y) * sensitivity; // inverted Y
    lastMouseX = x;
    lastMouseY = y;

    yaw += dx;
    pitch += dy;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

void onMouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // encyclopedia picking
        if (encyclopediaPicked) {
            encyclopediaDropPos = encyclopediaPos;
            encyclopediaPicked = false;
            encyclopediaFalling = true;
            encyclopediaVelocity = 0.0f;
        }
        else {
            glm::vec3 toBook = glm::normalize(encyclopediaPos - cameraPos);
            float dot = glm::dot(glm::normalize(cameraFront), toBook);
            float dist = glm::length(encyclopediaPos - cameraPos);
            if (dot > 0.90f && dist < 15.0f) {
                encyclopediaPicked = true;
                encyclopediaFalling = false;
            }
        }

        // succulent picking
        glm::vec3 toSucculent = glm::normalize(succulentPos - cameraPos);
        float dotSucculent = glm::dot(glm::normalize(cameraFront), toSucculent);
        float distSucculent = glm::length(succulentPos - cameraPos);
        if (dotSucculent > 0.90f && distSucculent < 15.0f && !succulentJumping) {
            succulentJumping = true;
            succulentJumpVelocity = 0.15f;
        }
    }

    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        // lamp picking
        glm::vec3 lampPos = glm::vec3(0.0f, 3.3f, 0.0f);
        glm::vec3 toLamp = glm::normalize(lampPos - cameraPos);
        float dotLamp = glm::dot(glm::normalize(cameraFront), toLamp);
        float distLamp = glm::length(lampPos - cameraPos);
        if (dotLamp > 0.85f && distLamp < 15.0f) {
            pointLightOn = !pointLightOn;
        }
    }
}

void updateProjection(int width, int height) {
    projMatrix = glm::perspective(
        glm::radians(60.0f),
        (float)width / (float)height,
        0.1f, 100.0f
    );
}

bool collisionEnabled = true;

void checkCollision(glm::vec3& newPos, const glm::vec3& oldPos) {
    if (!collisionEnabled) return;

    if (newPos.x < -7.5f) newPos.x = -7.5f;
    if (newPos.x > 7.5f) newPos.x = 7.5f;
    if (newPos.z < -9.5f) newPos.z = -9.5f;
    if (newPos.z > 9.5f) newPos.z = 9.5f;
    if (newPos.y < -3.0f) newPos.y = -3.0f;
    if (newPos.y > 2.5f) newPos.y = 2.5f;

    // collision with teacher's desk
    glm::vec3 deskPos = glm::vec3(-3.0f, -3.5f, -6.0f);
    float distToDesk = glm::length(glm::vec2(newPos.x - deskPos.x, newPos.z - deskPos.z));
    if (distToDesk < 1.5f) {
        newPos = oldPos;
    }
}

glm::mat4 getViewMatrix() {
    return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 getProjectionMatrix() {
    return projMatrix;
}

glm::vec3 getCameraPos() {
    return cameraPos;
}

glm::vec3 getCameraFront() {
    return cameraFront;
}