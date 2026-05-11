#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform mat4 Mmatrix;
uniform mat4 texMatrix;
uniform int useRotation;

smooth out vec2 texCoord_v;
smooth out vec3 fragPos_v;

void main() {
    gl_Position = PVMmatrix * vec4(position, 1.0);
    fragPos_v = vec3(Mmatrix * vec4(position, 1.0));

    if (useRotation == 1) {
        vec2 centered = texCoord - vec2(0.5, 0.5); //move the center, so we rotate arount (0,0)
        texCoord_v = vec2(texMatrix * vec4(centered, 0.0, 1.0)) + vec2(0.5, 0.5); //rotate and get the center back
    } else {
        texCoord_v = texCoord;
    }
}
