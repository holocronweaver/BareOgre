#version 150

in vec4 passPos;
in vec3 passNormal;
in vec2 passUV;

out vec4 fragColour;

uniform vec4 sceneColour;
uniform vec4 diffuseColour;

uniform vec4 surfaceToLightVector;

// Model a single light using naive Phong lighting and no texture.
void main()
{
    fragColour = sceneColour;
    fragColour += diffuseColour * (normalize(surfaceToLightVector) * vec4(passNormal, 0));
}
