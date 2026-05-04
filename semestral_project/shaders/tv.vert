#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform int currentFrame;
uniform int cols;
uniform int rows;

smooth out vec2 texCoord_v;

void main() {
    int col = currentFrame % cols;
    int row = currentFrame / cols;

    float frameW = 1.0 / float(cols);
    float frameH = 1.0 / float(rows);

    // map texcoord to current frame
    texCoord_v = vec2(
        (float(col) + texCoord.x) * frameW,
        1.0 - (float(row) + 1.0 - texCoord.y) * frameH
    );

    gl_Position = PVMmatrix * vec4(position, 1.0);
}