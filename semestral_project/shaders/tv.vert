#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform mat4 Mmatrix;
uniform int currentFrame;
uniform int cols;
uniform int rows;

smooth out vec2 texCoord_v;
smooth out vec3 fragPos_v;

void main() {
    int col = currentFrame % cols;
    int row = currentFrame / cols;

    float frameW = 1.0 / float(cols);
    float frameH = 1.0 / float(rows);

    texCoord_v = vec2(
        (float(col) + texCoord.x) * frameW,
        1.0 - (float(row) + 1.0 - texCoord.y) * frameH
    );

    gl_Position = PVMmatrix * vec4(position, 1.0);
    fragPos_v = vec3(Mmatrix * vec4(position, 1.0));
}