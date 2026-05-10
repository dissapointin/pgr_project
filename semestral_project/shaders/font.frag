#version 140

uniform sampler2D texSampler;
smooth in vec2 texCoord_v;
out vec4 fragColor;

void main() {
    vec4 texColor = texture(texSampler, texCoord_v);
    if (texColor.a < 0.1)
        discard;
    fragColor = texColor;
}