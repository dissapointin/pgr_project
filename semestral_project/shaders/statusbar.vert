#version 140

in vec3 position;
uniform float fillAmount;

smooth out float xPos_v;

void main() {
    gl_Position = vec4(position, 1.0);
    xPos_v = position.x;
}