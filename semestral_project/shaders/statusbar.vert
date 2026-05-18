//----------------------------------------------------------------------------------------
/**
 * \file    statusbar.vert
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Implementation of vertex shader for the status bar object.
 */
//----------------------------------------------------------------------------------------

#version 140

in vec3 position;
uniform float fillAmount;

smooth out float xPos_v;

void main() {
    gl_Position = vec4(position, 1.0);
    xPos_v = position.x;
}