#pragma once
#include "pgr.h"

struct BoardGeometry {
    GLuint vao;
    GLuint vbo;
    GLuint shaderProgram;
    GLint posLocation;
    GLint normalLocation;
    GLint PVMmatrixLocation;
    GLint MmatrixLocation;
    GLint normalMatrixLocation;
    GLint shininessLocation;
    GLint cameraPosLocation;
    GLint dirLightDirLocation;
    GLint dirLightColorLocation;
    GLint pointLightPosLocation;
    GLint pointLightColorLocation;
    GLint pointLightConstantLocation;
    GLint pointLightLinearLocation;
    GLint pointLightQuadraticLocation;
    GLint spotLightPosLocation;
    GLint spotLightDirLocation;
    GLint spotLightColorLocation;
    GLint spotLightCutoffLocation;
    GLint spotLightOuterCutoffLocation;
};

void initBoard();
void drawBoard();