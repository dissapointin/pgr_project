//----------------------------------------------------------------------------------------
/**
 * \file    board.frag
 * \author  Veronika Ihnashkina
 * \date    Summer semestr of 2025/2026 school year
 * \brief   Implementation of fragment shader for the board object.
 */
//----------------------------------------------------------------------------------------

#version 140

uniform float fogStart;
uniform float fogEnd;
uniform vec3 fogColor;
uniform int fogEnabled;

uniform vec3 dirLightDir;
uniform vec3 dirLightColor;
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;
uniform float pointLightConstant;
uniform float pointLightLinear;
uniform float pointLightQuadratic;
uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform vec3 spotLightColor;
uniform float spotLightCutoff;
uniform float spotLightOuterCutoff;
uniform float shininess;
uniform vec3 cameraPos;

smooth in vec3 fragPos_v;
smooth in vec3 normal_v;
out vec4 fragColor;

/// @brief Calculate directional light contribution (Phong)
/// @param norm surface normal
/// @param viewDir direction to camera
/// @param diffuse diffuse color of surface
vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffuse) {
    vec3 lightD  = normalize(dirLightDir);
    vec3 reflDir = reflect(-lightD, norm);
    vec3 ambient = diffuse * 0.15;
    float diff   = max(dot(norm, lightD), 0.0);
    float spec   = pow(max(dot(viewDir, reflDir), 0.0), shininess);
    return ambient * dirLightColor
         + diff * diffuse * dirLightColor
         + spec * vec3(0.05) * dirLightColor;
}

/// @brief Calculate point light contribution with attenuation
/// @param norm surface normal  
/// @param viewDir direction to camera
/// @param diffuse diffuse color of surface
vec3 calcPointLight(vec3 norm, vec3 viewDir, vec3 diffuse) {
    vec3 lightD  = normalize(pointLightPos - fragPos_v);
    vec3 reflDir = reflect(-lightD, norm);
    float distance    = length(pointLightPos - fragPos_v);
    float attenuation = 1.0 / (pointLightConstant
                              + pointLightLinear * distance
                              + pointLightQuadratic * distance * distance);
    vec3 ambient = diffuse * 0.05;
    float diff   = max(dot(norm, lightD), 0.0);
    float spec   = pow(max(dot(viewDir, reflDir), 0.0), shininess);
    return (ambient + diff * diffuse + spec * vec3(0.05))
           * pointLightColor * attenuation;
}

/// @brief Calculate spot light contribution with soft edges
/// @param norm surface normal
/// @param viewDir direction to camera
/// @param diffuse diffuse color of surface
vec3 calcSpotLight(vec3 norm, vec3 viewDir, vec3 diffuse) {
    vec3 lightD   = normalize(spotLightPos - fragPos_v);
    vec3 reflDir  = reflect(-lightD, norm);
    float theta   = dot(lightD, normalize(-spotLightDir));
    float epsilon = spotLightCutoff - spotLightOuterCutoff;
    float intensity = clamp((theta - spotLightOuterCutoff) / epsilon, 0.0, 1.0);
    float diff = max(dot(norm, lightD), 0.0);
    float spec = pow(max(dot(viewDir, reflDir), 0.0), shininess);
    return (diff * diffuse + spec * vec3(0.1))
           * spotLightColor * intensity;
}

void main() {
    vec3 norm    = normalize(normal_v);
    vec3 viewDir = normalize(cameraPos - fragPos_v);

    vec3 diffuse;
    if (norm.y > 0.5) {
        diffuse = vec3(0.4, 0.25, 0.1);
    } else if (norm.z > 0.5 && fragPos_v.y > -1.0) {
        diffuse = vec3(0.15, 0.35, 0.20);
    } else {
        diffuse = vec3(0.4, 0.25, 0.1);
    }

    vec3 result = calcDirLight(norm, viewDir, diffuse)
                + calcPointLight(norm, viewDir, diffuse)
                + calcSpotLight(norm, viewDir, diffuse);

    if (fogEnabled == 1) {
        float dist = length(cameraPos - fragPos_v);
        float fogFactor = clamp((fogEnd - dist) / (fogEnd - fogStart), 0.0, 1.0);
        fragColor = vec4(mix(fogColor, result, fogFactor), 1.0);
    } else {
        fragColor = vec4(result, 1.0);
    }
}
