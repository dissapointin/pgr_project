#version 140

uniform mat4 PVMmatrix; // projection view
uniform mat4 Mmatrix; // model (world view, not screen view)
uniform mat4 normalMatrix; // transpose(inverse(Matrix)) for normals

in vec3 position; 
in vec3 normal;

smooth out vec3 fragPos_v; // to pass to frag shader
smooth out vec3 normal_v;

void main() {
    gl_Position = PVMmatrix * vec4(position, 1);
    fragPos_v = vec3(Mmatrix * vec4(position, 1));
    normal_v = normal;
}