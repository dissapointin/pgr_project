//----------------------------------------------------------------------------------------
/**
 * \file    font.frag
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Implementation of fragment shader for the font textures.
 */
//----------------------------------------------------------------------------------------

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