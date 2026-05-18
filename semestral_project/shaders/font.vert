//----------------------------------------------------------------------------------------
/**
 * \file    font.vert
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Implementation of vertex shader for the font textures.
 */
//----------------------------------------------------------------------------------------

#version 140

in vec3 position;
in vec2 texCoord;

uniform mat4 PVMmatrix;
uniform ivec2 charIndex;

smooth out vec2 texCoord_v;

void main() {
    float imgW = 647.0;
    float imgH = 385.0;
    float charW = 36.0;
    float charH = 36.0;
    float stride = 43.0;
    float startX = 43.0;
    float startY = 40.0;

    float left   = (startX + float(charIndex.x) * stride) / imgW;
    float right  = left + charW / imgW;
    float top    = (startY + float(charIndex.y) * stride) / imgH;
    float bottom = top + charH / imgH;

    texCoord_v = vec2(
        left + texCoord.x * (right - left),
        1.0 - (top + (1.0 - texCoord.y) * (bottom - top))
    );

    gl_Position = PVMmatrix * vec4(position, 1.0);
}