// DESK AND CHAIR MODELS: https://sketchfab.com/3d-models/school-desk-a74180ee97bb4917b24cd48580663b44
// SHELF MODEL: https://sketchfab.com/3d-models/kallax-shelf-e2d30124563947608a500df26f77b659
// TRASH BIN MODEL: https://sketchfab.com/3d-models/waste-bins-f25c067806dd45aba900502e9c470e7a
// CEILING LAMP MODEL: https://sketchfab.com/3d-models/ceiling-lamp-673-ffd6cdb14ab849658c45ed4e3676e7d1
// ENCYCLOPEDIA MODEL: https://sketchfab.com/3d-models/book-encyclopedia-0487cb088c244d02a736cb337e65778c
// WINDOW MODEL: https://sketchfab.com/3d-models/window-2811eb28fa7a49b4adb86b948f75ff37
// TEACHER'S DESK MODEL: https://sketchfab.com/3d-models/teachers-desk-9ee8608e76704de7aa67d022c81e42cc
// TV MODEL: https://sketchfab.com/3d-models/led-tv-low-poly-free-c3b53886b18247b79460b4992ea071d1
// SKYBOX PARK 2: https://humus.name/index.php?page=Textures&start=128
// CURTAINS MODEL: https://sketchfab.com/3d-models/curtains-155c08c127d649758fa074a216bd33cf

#include "pgr.h"
#include "render.h"
#include "camera.h"
#include "fan.h"
#include "config.h"
#include <iostream>

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
    case 'f': case 'F': spotLightOn = !spotLightOn; break;
    case 'g': case 'G': fogEnabled = !fogEnabled; break;
    case 'z': case 'Z': zPressed = true; break;
    case 'r': case 'R': loadConfig("config.txt"); break;
	case 't': case 'T': autoTime = !autoTime; break;  // on/off auto time change
    case 'y': case 'Y': timeOfDay += 0.05f; if (timeOfDay > 1.0f) timeOfDay = 0.0f; break; // forward
	case 'u': case 'U': timeOfDay -= 0.05f; if (timeOfDay < 0.0f) timeOfDay = 1.0f; break; // backward
    case '1': if (zPressed) fanMode = FAN_STOPPED;     else setCameraStatic(0); break;
    case '2': if (zPressed) fanMode = FAN_CIRCLE;      else setCameraStatic(1); break;
    case '3': if (zPressed) fanMode = FAN_CATMULL_ROM; else setCameraFree();    break;
    case '4': if (zPressed) fanMode = FAN_BEZIER;      break;
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
    updateScene();  // from render.cpp - update animations
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);  // ~60 FPS
}

/// @brief Window resize callback
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    updateProjection(width, height);  // from camera.cpp
}

/// @brief Keyboard key release callback
void keyboardUp(unsigned char key, int x, int y) {
    if (key == 'z' || key == 'Z') zPressed = false;
}

/// @brief Menu callback for fan animation modes and toggles
void menu(int value) {
    switch (value) {
    case 1: setCameraStatic(0); break;
    case 2: setCameraStatic(1); break;
    case 3: setCameraFree();    break;
    case 4: spotLightOn = !spotLightOn; break;
    case 5: fogEnabled = !fogEnabled; break;
    case 6: fanMode = FAN_CIRCLE; break;
    case 7: fanMode = FAN_CATMULL_ROM; break;
    case 8: fanMode = FAN_BEZIER; break;
    case 9: fanMode = FAN_STOPPED; break;
    }
}

int main(int argc, char** argv) {
    // init pgr framework + glut window
    glutInit(&argc, argv);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow(WINDOW_TITLE);

    glutCreateMenu(menu);
    glutAddMenuEntry("Camera 1", 1);
    glutAddMenuEntry("Camera 2", 2);
    glutAddMenuEntry("Free camera", 3);
    glutAddMenuEntry("Toggle flashlight", 4);
    glutAddMenuEntry("Toggle fog", 5);
    glutAddMenuEntry("Fan: circle", 6);
    glutAddMenuEntry("Fan: Catmull-Rom", 7);
    glutAddMenuEntry("Fan: Bezier", 8);
    glutAddMenuEntry("Fan: stop", 9);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    if (!pgr::initialize(pgr::OGL_VER_MAJOR, pgr::OGL_VER_MINOR)) {
        exit(1);
    }

    // register callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(keyboardSpecial);
    glutPassiveMotionFunc(mouseMotion);
    glutMouseFunc(mouse);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, timer, 0);

    // init scene
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    initScene();   // from render.cpp
    initCamera();  // from camera.cpp
    loadConfig("config.txt");

    glutMainLoop();
    return 0;
}