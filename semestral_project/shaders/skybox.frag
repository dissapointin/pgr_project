//----------------------------------------------------------------------------------------
/**
 * \file    skybox.frag
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Implementation of fragment shader for the skybox.
 */
//----------------------------------------------------------------------------------------

#version 140

smooth in vec3 texCoord_v;
uniform samplerCube skybox;
out vec4 fragColor;

void main() {
    fragColor = texture(skybox, vec3(texCoord_v.x, -texCoord_v.y, texCoord_v.z));
}