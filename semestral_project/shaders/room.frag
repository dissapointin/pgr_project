#version 140

// DIRECTIONAL LIGHT
uniform vec3 dirLightDir; // direction !TO! the light
uniform vec3 dirLightColor; // color of the light

// POINT LIGHT
uniform vec3 pointLightPos;
uniform vec3 pointLightColor;
uniform float pointLightConstant;
uniform float pointLightLinear;
uniform float pointLightQuadratic;

// SPOT LIGHT
uniform vec3 spotLightPos;
uniform vec3 spotLightDir;
uniform vec3 spotLightColor;
uniform float spotLightCutoff;      // cos of inner angle
uniform float spotLightOuterCutoff; // cos of outer angle

// Material of objects that are reacting to the light
uniform float shininess;

uniform vec3 cameraPos;

smooth in vec3 fragPos_v;
smooth in vec3 normal_v;
out vec4 fragColor;

uniform sampler2D texSampler; // floor
uniform sampler2D wallSampler; // walls
uniform sampler2D ceilingSampler; // ceiling
smooth in vec2 texCoord_v;

vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 diffuse) {
    vec3 lightD = normalize(dirLightDir);
    vec3 reflDir = reflect(-lightD, norm);

    vec3 ambient  = diffuse * 0.15;
    float diff    = max(dot(norm, lightD), 0.0);
    float spec    = pow(max(dot(viewDir, reflDir), 0.0), shininess);

    return ambient * dirLightColor
         + diff * diffuse * dirLightColor
         + spec * vec3(0.05) * dirLightColor;
}

vec3 calcPointLight(vec3 norm, vec3 viewDir, vec3 diffuse) {
    vec3 lightD  = normalize(pointLightPos - fragPos_v);
    vec3 reflDir = reflect(-lightD, norm);

    float distance    = length(pointLightPos - fragPos_v);
    float attenuation = 1.0 / (pointLightConstant
                              + pointLightLinear * distance
                              + pointLightQuadratic * distance * distance);

    vec3 ambient  = diffuse * 0.05;
    float diff    = max(dot(norm, lightD), 0.0);
    float spec    = pow(max(dot(viewDir, reflDir), 0.0), shininess);

    return (ambient + diff * diffuse + spec * vec3(0.05))
           * pointLightColor * attenuation;
}

vec3 calcSpotLight(vec3 norm, vec3 viewDir, vec3 diffuse) {
    vec3 lightD  = normalize(spotLightPos - fragPos_v);
    vec3 reflDir = reflect(-lightD, norm);

    float theta   = dot(lightD, normalize(-spotLightDir));
    float epsilon = spotLightCutoff - spotLightOuterCutoff;
    float intensity = clamp((theta - spotLightOuterCutoff) / epsilon, 0.0, 1.0);

    float diff = max(dot(norm, lightD), 0.0);
    float spec = pow(max(dot(viewDir, reflDir), 0.0), shininess);

    return (diff * diffuse + spec * vec3(0.1))
           * spotLightColor * intensity;
}

void main() {
    // normalize to make them 1 long
    vec3 norm = normalize(normal_v);
    vec3 lightD = normalize(dirLightDir);
    vec3 viewDir = normalize(cameraPos - fragPos_v);
    // reflect to calculate normal of reflection
    vec3 reflDir = reflect(-lightD, norm);

    vec3 texColor;
    if (abs(norm.y) > 0.5) {
        if (norm.y > 0.0)
            texColor = vec3(texture(texSampler, texCoord_v * 3.0));     // floor
        else
            texColor = vec3(texture(ceilingSampler, texCoord_v * 2.0)); // ceiling
    } else {
        texColor = vec3(texture(wallSampler, texCoord_v * 2.0));        // walls
    }
    vec3 diffuse = texColor;

    vec3 result = calcDirLight(norm, viewDir, diffuse)
                + calcPointLight(norm, viewDir, diffuse)
                + calcSpotLight(norm, viewDir, diffuse);

    fragColor = vec4(result, 1.0);
}