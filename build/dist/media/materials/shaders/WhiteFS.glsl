#version 150

in vec4 passPos;
in vec3 passNormal;
in vec2 passUV;

out vec4 fragColour;

// Shades everything white.
void main()
{
    fragColour = vec4(1);
}

