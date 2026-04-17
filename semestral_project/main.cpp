#include "pgr.h"
#include "render.h"
#include "camera.h"

// --- Window settings ---
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const char* WINDOW_TITLE = "Japanese Classroom";

// --- Callbacks ---

/// @brief Display callback - called every frame
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    drawScene();
    glutSwapBuffers();
}

/// @brief Keyboard callback
/// @param key pressed key
/// @param x mouse x position
/// @param y mouse y position
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        exit(0);
        break;
    case '1': setCameraStatic(0); break;
    case '2': setCameraStatic(1); break;
    case '3': setCameraFree();    break;
    }
    onKeyPress(key);  // z camera.cpp - pohyb WASD
}

/// @brief Special keys callback (arrows)
void keyboardSpecial(int key, int x, int y) {
    onSpecialKeyPress(key);  // z camera.cpp
}

/// @brief Mouse movement callback
/// @param x mouse x position
/// @param y mouse y position
void mouseMotion(int x, int y) {
    onMouseMove(x, y);  // z camera.cpp
}

/// @brief Mouse click callback
void mouse(int button, int state, int x, int y) {
    onMouseClick(button, state, x, y);
}

/// @brief Timer callback - for animations
/// @param value unused
void timer(int value) {
    updateScene();  // z render.cpp - update animaci
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  // ~60 FPS
}

/// @brief Window resize callback
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    updateProjection(width, height);  // z camera.cpp
}

int main(int argc, char** argv) {
    // init pgr framework + glut window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR)) {
        exit(1);
    }

    // register callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpecial);
    glutPassiveMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);

    // init scene
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    initScene();   // z render.cpp
    initCamera();  // z camera.cpp

    glutMainLoop();
    return 0;
}