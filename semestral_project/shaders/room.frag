#version 140

// LIGHT
uniform vec3 lightDir; // direction !TO! the light
uniform vec3 lightColor; // color of the light

// Material of objects that are reacting to the light
uniform float shininess;

uniform vec3 cameraPos;

smooth in vec3 fragPos_v;
smooth in vec3 normal_v;
out vec4 fragColor;

void main() {
    // normalize to make them 1 long
    vec3 norm = normalize(normal_v);
    vec3 lightD = normalize(lightDir);
    vec3 viewDir = normalize(cameraPos - fragPos_v);
    // reflect to calculate normal of reflection
    vec3 reflDir = reflect(-lightD, norm);

    // chose color dependent on the normal
    vec3 diffuse;
    if (abs(norm.y) > 0.5) {
        if (norm.y > 0.0)
            diffuse = vec3(0.55, 0.35, 0.20); // ceiling is white
        else
            diffuse = vec3(0.95, 0.95, 0.90); // floor is brown
    } else {
        diffuse = vec3(0.85, 0.78, 0.65); // walls are beige
    }

    vec3 ambient  = diffuse * 0.3;
    vec3 specular = vec3(0.05);

    float diff = max(dot(norm, lightD), 0.0);
    vec3 diffuseResult = diff * diffuse * lightColor;

    float spec = pow(max(dot(viewDir, reflDir), 0.0), shininess);
    vec3 specularResult = spec * specular * lightColor;

    vec3 ambientResult  = ambient * lightColor;

    vec3 result = ambientResult + diffuseResult + specularResult;
    fragColor = vec4(result, 1.0);
}