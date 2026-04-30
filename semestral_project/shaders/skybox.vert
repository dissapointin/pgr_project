#version 140

in vec3 position;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

smooth out vec3 texCoord_v;

void main() {
    texCoord_v = position;
    vec4 pos = projMatrix * viewMatrix * vec4(position, 1.0);
    gl_Position = pos.xyww;
}