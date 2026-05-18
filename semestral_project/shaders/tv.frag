//----------------------------------------------------------------------------------------
/**
 * \file    tv.frag
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Implementation of fragment shader for the TV monitor texture.
 */
//----------------------------------------------------------------------------------------

#version 140

uniform sampler2D texSampler;
uniform int fogEnabled;
uniform float fogStart;
uniform float fogEnd;
uniform vec3 fogColor;
uniform vec3 cameraPos;

smooth in vec2 texCoord_v;
smooth in vec3 fragPos_v;
out vec4 fragColor;

void main() {
    vec4 texColor = texture(texSampler, texCoord_v);
    vec3 result = texColor.rgb;

    if (fogEnabled == 1) {
        float dist = length(cameraPos - fragPos_v);
        float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);
        result = mix(fogColor, result, fogFactor);
    }

    fragColor = vec4(result, texColor.a);
}
