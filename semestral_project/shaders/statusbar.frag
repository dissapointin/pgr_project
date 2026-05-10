#version 140

uniform vec3 barColor;
uniform float fillAmount;
smooth in float xPos_v;
out vec4 fragColor;

void main() {
    // bar goes from -0.9 to 0.9, fill based on timeOfDay
    float barStart = -0.9;
    float barEnd   =  0.9;
    float filled = barStart + (barEnd - barStart) * fillAmount;

    if (xPos_v > filled) {
        // unfilled part - dark grey
        fragColor = vec4(0.2, 0.2, 0.2, 0.8);
    } else {
        // filled part - time color
        fragColor = vec4(barColor, 0.9);
    }
}