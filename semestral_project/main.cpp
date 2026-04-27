// DESK AND CHAIR MODELS: https://sketchfab.com/3d-models/school-desk-a74180ee97bb4917b24cd48580663b44
// SHELF MODEL: https://sketchfab.com/3d-models/kallax-shelf-e2d30124563947608a500df26f77b659
// TRASH BIN MODEL: https://sketchfab.com/3d-models/waste-bins-f25c067806dd45aba900502e9c470e7a
// CEILING LAMP MODEL: https://sketchfab.com/3d-models/ceiling-lamp-673-ffd6cdb14ab849658c45ed4e3676e7d1
// ENCYCLOPEDIA MODEL: https://sketchfab.com/3d-models/book-encyclopedia-0487cb088c244d02a736cb337e65778c
// WINDOW MODEL: https://sketchfab.com/3d-models/window-2811eb28fa7a49b4adb86b948f75ff37
// TEACHER'S DESK MODEL: https://sketchfab.com/3d-models/teachers-desk-9ee8608e76704de7aa67d022c81e42cc

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
    case 'f': case 'F': spotLightOn = !spotLightOn; break;
    }
    onKeyPress(key);  // from camera.cpp -  WASD
}

/// @brief Special keys callback (arrows)
void keyboardSpecial(int key, int x, int y) {
    onSpecialKeyPress(key);  // from camera.cpp
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