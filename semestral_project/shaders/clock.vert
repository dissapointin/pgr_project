#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform mat4 texMatrix;
uniform int useRotation;

smooth out vec2 texCoord_v;

void main() {
    if (useRotation == 1) {
        vec4 rotatedPos = texMatrix * vec4(position.xy, 0.0, 1.0);
        gl_Position = PVMmatrix * vec4(rotatedPos.xy, position.z, 1.0);
    } else {
        gl_Position = PVMmatrix * vec4(position, 1.0);
    }
    texCoord_v = texCoord;
}