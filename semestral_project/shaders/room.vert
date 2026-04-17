#version 140

uniform mat4 PVMmatrix;
in vec3 position;
in vec3 normal;

smooth out vec3 normal_v;

void main() {
    gl_Position = PVMmatrix * vec4(position, 1);
    normal_v = normal;
}