#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform float time;

smooth out vec2 texCoord_v;

void main() {
    gl_Position = PVMmatrix * vec4(position, 1.0);
    // animate texture coordinates
    texCoord_v = texCoord + vec2(time * 0.02, time * 0.01);
}