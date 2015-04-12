#version 150

in vec4 vertex;
in vec3 normal;
in vec2 uv0;

out vec4 passPos;
out vec3 passNormal;
out vec2 passUV;

uniform mat4 worldViewProj;

void main()
{
    gl_Position = worldViewProj * vertex;
    passPos = vertex;
    passUV = uv0;
    passNormal = normal;
}
