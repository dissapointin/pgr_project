#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform float rotationAngle;
uniform int useRotation;

smooth out vec2 texCoord_v;

void main() {
    vec2 pos = position.xy;

    if (useRotation == 1) {
        float rad = radians(rotationAngle);
        float cosA = cos(rad);
        float sinA = sin(rad);
        // rotate around center (0.5, 0.5) in texture space
        vec2 centered = pos;
        pos.x = centered.x * cosA - centered.y * sinA;
        pos.y = centered.x * sinA + centered.y * cosA;
    }

    gl_Position = PVMmatrix * vec4(pos, position.z, 1.0);
    texCoord_v = texCoord;
}