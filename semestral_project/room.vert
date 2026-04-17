#version 140

in vec3 position;
in vec3 normal;

uniform mat4 PVMmatrix;

void main() {
    gl_Position = PVMmatrix * vec4(position, 1.0);
}