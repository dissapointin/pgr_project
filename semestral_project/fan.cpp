#include "fan.h"
#include "camera.h"
#include "render.h"
#include <cmath>
#include <vector>

extern bool spotLightOn;
extern bool fogEnabled;
extern bool pointLightOn;

FanGeometry fan;
FanMode fanMode = FAN_STOPPED;
float fanHeadAngle = 0.0f;  // head rotation (level 2)
float fanBladeAngle = 0.0f;  // blade rotation (level 3)
glm::vec3 fanPosition = glm::vec3(2.0f, -0.7f, 9.0f);
float fanTime = 0.0f;

// Catmull-Rom control points 
glm::vec3 catmullPoints[] = {
    glm::vec3(2.0f, -3.5f,  0.0f),
    glm::vec3(5.0f, -3.5f, -4.0f),
    glm::vec3(0.0f, -3.5f, -7.0f),
    glm::vec3(-5.0f, -3.5f, -4.0f),
    glm::vec3(-5.0f, -3.5f,  4.0f),
    glm::vec3(0.0f, -3.5f,  7.0f),
    glm::vec3(5.0f, -3.5f,  4.0f),
    glm::vec3(2.0f, -3.5f,  0.0f),
};
const int CATMULL_COUNT = 8;

// Bezier control points (cubic)
static const glm::vec3 bezierPoints[] = {
    glm::vec3(-5.0f, -3.5f, -5.0f),
    glm::vec3(5.0f, -3.5f, -5.0f),
    glm::vec3(-5.0f, -3.5f,  5.0f),
    glm::vec3(5.0f, -3.5f,  5.0f),
};

// Create simple cylinder vertices 
static std::vector<float> makeCylinder(float r, float h, int seg, glm::vec3 color) {
    std::vector<float> v;
    for (int i = 0; i < seg; i++) {
        float a0 = 2 * M_PI * i / seg;
        float a1 = 2 * M_PI * (i + 1) / seg;
        float x0 = r * cos(a0), z0 = r * sin(a0);
        float x1 = r * cos(a1), z1 = r * sin(a1);
        // side face
        float nx0 = cos(a0), nz0 = sin(a0);
        float nx1 = cos(a1), nz1 = sin(a1);
        // tri 1
        v.insert(v.end(), { x0,0,z0, nx0,0,nz0 });
        v.insert(v.end(), { x1,0,z1, nx1,0,nz1 });
        v.insert(v.end(), { x0,h,z0, nx0,0,nz0 });
        // tri 2
        v.insert(v.end(), { x1,0,z1, nx1,0,nz1 });
        v.insert(v.end(), { x1,h,z1, nx1,0,nz1 });
        v.insert(v.end(), { x0,h,z0, nx0,0,nz0 });
    }
    return v;
}

// Create blade (flat rectangle) 
static std::vector<float> makeBlade(float w, float l) {
    std::vector<float> v = {
        -w / 2, 0, 0,    0,1,0,
         w / 2, 0, 0,    0,1,0,
         w / 2, 0, l,    0,1,0,
        -w / 2, 0, 0,    0,1,0,
         w / 2, 0, l,    0,1,0,
        -w / 2, 0, l,    0,1,0,
        -w / 2, 0, 0,    0,-1,0,
         w / 2, 0, l,    0,-1,0,
         w / 2, 0, 0,    0,-1,0,
        -w / 2, 0, 0,    0,-1,0,
        -w / 2, 0, l,    0,-1,0,
         w / 2, 0, l,    0,-1,0,
    };
    return v;
}

static void uploadMesh(GLuint& vao, GLuint& vbo, const std::vector<float>& v,
    int& count, const FanGeometry& f) {
    count = v.size() / 6;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(float), v.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(f.posLocation);
    glVertexAttribPointer(f.posLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(f.normalLocation);
    glVertexAttribPointer(f.normalLocation, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);
}

void initFan() {
    GLuint shaders[] = {
        pgr::createShaderFromFile(GL_VERTEX_SHADER,   "shaders/model.vert"),
        pgr::createShaderFromFile(GL_FRAGMENT_SHADER, "shaders/model.frag"),
        0
    };
    fan.shaderProgram = pgr::createProgram(shaders);

    fan.posLocation = glGetAttribLocation(fan.shaderProgram, "position");
    fan.normalLocation = glGetAttribLocation(fan.shaderProgram, "normal");
    fan.PVMmatrixLocation = glGetUniformLocation(fan.shaderProgram, "PVMmatrix");
    fan.MmatrixLocation = glGetUniformLocation(fan.shaderProgram, "Mmatrix");
    fan.normalMatrixLocation = glGetUniformLocation(fan.shaderProgram, "normalMatrix");
    fan.shininessLocation = glGetUniformLocation(fan.shaderProgram, "shininess");
    fan.cameraPosLocation = glGetUniformLocation(fan.shaderProgram, "cameraPos");
    fan.dirLightDirLocation = glGetUniformLocation(fan.shaderProgram, "dirLightDir");
    fan.dirLightColorLocation = glGetUniformLocation(fan.shaderProgram, "dirLightColor");
    fan.pointLightPosLocation = glGetUniformLocation(fan.shaderProgram, "pointLightPos");
    fan.pointLightColorLocation = glGetUniformLocation(fan.shaderProgram, "pointLightColor");
    fan.pointLightConstantLocation = glGetUniformLocation(fan.shaderProgram, "pointLightConstant");
    fan.pointLightLinearLocation = glGetUniformLocation(fan.shaderProgram, "pointLightLinear");
    fan.pointLightQuadraticLocation = glGetUniformLocation(fan.shaderProgram, "pointLightQuadratic");
    fan.spotLightPosLocation = glGetUniformLocation(fan.shaderProgram, "spotLightPos");
    fan.spotLightDirLocation = glGetUniformLocation(fan.shaderProgram, "spotLightDir");
    fan.spotLightColorLocation = glGetUniformLocation(fan.shaderProgram, "spotLightColor");
    fan.spotLightCutoffLocation = glGetUniformLocation(fan.shaderProgram, "spotLightCutoff");
    fan.spotLightOuterCutoffLocation = glGetUniformLocation(fan.shaderProgram, "spotLightOuterCutoff");
    fan.diffuseColorLocation = glGetUniformLocation(fan.shaderProgram, "diffuseColor");
    fan.fogEnabledLocation = glGetUniformLocation(fan.shaderProgram, "fogEnabled");
    fan.fogStartLocation = glGetUniformLocation(fan.shaderProgram, "fogStart");
    fan.fogEndLocation = glGetUniformLocation(fan.shaderProgram, "fogEnd");
    fan.fogColorLocation = glGetUniformLocation(fan.shaderProgram, "fogColor");

    // base: thick cylinder
    auto baseVerts = makeCylinder(0.15f, 0.3f, 16, glm::vec3(0.3f));
    uploadMesh(fan.baseVao, fan.baseVbo, baseVerts, fan.baseVertexCount, fan);

    // head: smaller cylinder
    auto headVerts = makeCylinder(0.1f, 0.2f, 16, glm::vec3(0.4f));
    uploadMesh(fan.headVao, fan.headVbo, headVerts, fan.headVertexCount, fan);

    // blade: one blade, draw 4 times rotated
    auto bladeVerts = makeBlade(0.05f, 0.4f);
    uploadMesh(fan.bladeVao, fan.bladeVbo, bladeVerts, fan.bladeVertexCount, fan);
}

// Draw one mesh part
static void drawPart(GLuint vao, int count, const glm::mat4& modelMat,
    const glm::vec3& color, float shininess) {
    glm::vec3 camPos = getCameraPos();
    glm::mat4 PVM = getProjectionMatrix() * getViewMatrix() * modelMat;
    glm::mat4 normalMat = glm::transpose(glm::inverse(modelMat));

    glUniformMatrix4fv(fan.PVMmatrixLocation, 1, GL_FALSE, glm::value_ptr(PVM));
    glUniformMatrix4fv(fan.MmatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMat));
    glUniformMatrix4fv(fan.normalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMat));
    glUniform3fv(fan.diffuseColorLocation, 1, glm::value_ptr(color));
    glUniform1f(fan.shininessLocation, shininess);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}

void drawFan() {
    glUseProgram(fan.shaderProgram);

    glm::vec3 camPos = getCameraPos();
    glm::vec3 camFront = getCameraFront();

    // lighting
    glUniform3fv(fan.dirLightDirLocation, 1, glm::value_ptr(getDirLightDir()));
    glUniform3fv(fan.dirLightColorLocation, 1, glm::value_ptr(getDirLightColor()));
    glm::vec3 pointColor = pointLightOn ? glm::vec3(1.0f, 0.9f, 0.7f) : glm::vec3(0.0f);
    glUniform3f(fan.pointLightPosLocation, 0.0f, 3.0f, 0.0f);
    glUniform3fv(fan.pointLightColorLocation, 1, glm::value_ptr(pointColor));
    glUniform1f(fan.pointLightConstantLocation, 1.0f);
    glUniform1f(fan.pointLightLinearLocation, 0.09f);
    glUniform1f(fan.pointLightQuadraticLocation, 0.032f);
    glUniform3fv(fan.spotLightPosLocation, 1, glm::value_ptr(camPos));
    glUniform3fv(fan.spotLightDirLocation, 1, glm::value_ptr(camFront));
    glm::vec3 spotColor = spotLightOn ? glm::vec3(1.0f) : glm::vec3(0.0f);
    glUniform3fv(fan.spotLightColorLocation, 1, glm::value_ptr(spotColor));
    glUniform1f(fan.spotLightCutoffLocation, glm::cos(glm::radians(12.5f)));
    glUniform1f(fan.spotLightOuterCutoffLocation, glm::cos(glm::radians(17.5f)));
    glUniform3fv(fan.cameraPosLocation, 1, glm::value_ptr(camPos));
    glUniform1i(glGetUniformLocation(fan.shaderProgram, "hasTexture"), 0);
    glUniform1f(glGetUniformLocation(fan.shaderProgram, "opacity"), 1.0f);
    glUniform1f(glGetUniformLocation(fan.shaderProgram, "specularStrength"), 0.3f);
    glUniform1i(fan.fogEnabledLocation, fogEnabled ? 1 : 0);
    glUniform1f(fan.fogStartLocation, 1.0f);
    glUniform1f(fan.fogEndLocation, 10.0f);
    glUniform3f(fan.fogColorLocation, 0.7f, 0.7f, 0.7f);

    // Base (level 1)
    glm::mat4 baseMatrix = glm::translate(glm::mat4(1.0f), fanPosition);
    baseMatrix = glm::rotate(baseMatrix, glm::radians(180.0f), glm::vec3(0, 1, 0));
    drawPart(fan.baseVao, fan.baseVertexCount, baseMatrix,
        glm::vec3(0.3f, 0.3f, 0.3f), 32.0f);

	// Head (level 2, rotate around Y)
    glm::mat4 headMatrix = glm::translate(baseMatrix, glm::vec3(0.0f, 0.3f, 0.0f));
    headMatrix = glm::rotate(headMatrix, glm::radians(fanHeadAngle), glm::vec3(0, 1, 0));
    drawPart(fan.headVao, fan.headVertexCount, headMatrix,
        glm::vec3(0.4f, 0.4f, 0.4f), 32.0f);

    // Blades (level 3, rotate around Z)
    for (int i = 0; i < 4; i++) {
        float bladeOffset = 90.0f * i;
        glm::mat4 bladeMatrix = glm::translate(headMatrix, glm::vec3(0.0f, 0.2f, 0.15f)); // одна точка для всіх
        bladeMatrix = glm::rotate(bladeMatrix, glm::radians(fanBladeAngle + bladeOffset), glm::vec3(0, 0, 1));
        bladeMatrix = glm::rotate(bladeMatrix, glm::radians(90.0f), glm::vec3(0, 1, 0));
        drawPart(fan.bladeVao, fan.bladeVertexCount, bladeMatrix,
            glm::vec3(0.6f, 0.5f, 0.3f), 16.0f);
    }
}

// Catmull-Rom interpolation 
static glm::vec3 catmullRom(float t) {
    int n = CATMULL_COUNT - 1;
    float scaled = t * n;
    int i = (int)scaled;
    if (i >= n) i = n - 1;
    float u = scaled - i;

    glm::vec3 p0 = catmullPoints[glm::max(i - 1, 0)];
    glm::vec3 p1 = catmullPoints[i];
    glm::vec3 p2 = catmullPoints[glm::min(i + 1, n)];
    glm::vec3 p3 = catmullPoints[glm::min(i + 2, n)];

    return 0.5f * ((2.0f * p1) +
        (-p0 + p2) * u +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * u * u +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * u * u * u);
}

// Bezier interpolation
static glm::vec3 bezier(float t) {
    float u = 1.0f - t;
    return u * u * u * bezierPoints[0]
        + 3 * u * u * t * bezierPoints[1]
        + 3 * u * t * t * bezierPoints[2]
        + t * t * t * bezierPoints[3];
}

void updateFan() {
    // head always oscillates
    fanHeadAngle = 45.0f * sin(glutGet(GLUT_ELAPSED_TIME) * 0.001f);

    // blades always spin
    fanBladeAngle += 3.0f;
    if (fanBladeAngle > 360.0f) fanBladeAngle -= 360.0f;

    if (fanMode == FAN_STOPPED) {
        fanPosition = glm::vec3(2.0f, -0.7f, 9.0f);
        return;
    }

    // when moving - on floor
    fanPosition.y = -3.5f;
    fanTime += 0.005f;
    if (fanTime > 1.0f) fanTime -= 1.0f;

    // blades always spin when moving
    fanBladeAngle += 5.0f;
    if (fanBladeAngle > 360.0f) fanBladeAngle -= 360.0f;

    // head oscillates
    fanHeadAngle = 45.0f * sin(fanTime * 2.0f * M_PI * 3.0f);

    glm::vec3 prevPos = fanPosition;

    if (fanMode == FAN_CIRCLE) {
        float r = 4.0f;
        float angle = fanTime * 2.0f * M_PI;
        fanPosition.x = r * cos(angle);
        fanPosition.z = r * sin(angle);
        fanPosition.y = -3.5f;
    }
    else if (fanMode == FAN_CATMULL_ROM) {
        fanPosition = catmullRom(fanTime);
    }
    else if (fanMode == FAN_BEZIER) {
        fanPosition = bezier(fanTime);
    }
}