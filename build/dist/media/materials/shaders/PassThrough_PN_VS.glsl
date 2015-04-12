#version 150

in vec4 vertex;
in vec3 normal;

out vec4 passPos;
out vec3 passNormal;

uniform mat4 worldViewProj;

void main()
{
    gl_Position = worldViewProj * vertex;
    passPos = vertex;
    passNormal = normal;
}
