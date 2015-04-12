#version 150

in vec4 vertex;

out vec4 passPos;

uniform mat4 worldViewProj;

void main()
{
    gl_Position = worldViewProj * vertex;
    passPos = vertex;
}
