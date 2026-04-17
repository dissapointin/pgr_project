#version 140

smooth in vec3 normal_v;
out vec4 fragColor;

void main() {
    fragColor = vec4(abs(normal_v), 1.0);
}