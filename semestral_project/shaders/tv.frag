#version 140

uniform sampler2D texSampler;
smooth in vec2 texCoord_v;
out vec4 fragColor;

void main() {
    fragColor = texture(texSampler, texCoord_v);
}