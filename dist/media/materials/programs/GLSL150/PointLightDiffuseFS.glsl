#version 150

in vec4 passPos;
in vec3 passNormal;
in vec2 passUV;

out vec4 fragColour;

uniform sampler2D texMap;

uniform vec4 sceneColour;
uniform vec4 diffuseColour;

uniform mat4 worldView;
uniform mat4 worldViewIT;

uniform vec4 lightPosV;
uniform vec4 lightAttenuationCoeff;

// Model a single light using naive Phong lighting.
void main()
{
    fragColour = sceneColour;

    vec3 posV = (worldView * passPos).xyz;
    vec3 normalV = normalize((worldViewIT * vec4(passNormal.xyz, 1.0)).xyz);
    vec3 surfaceToLightV = lightPosV.xyz - posV;

    fragColour = pointLightDiffuse(posV, normalV, surfaceToLightV,
                                   lightAttenuationCoeff, diffuseColour.xyz);

    // fragColour += diffuseColour * (normalize(surfaceToLightVector) * vec4(passNormal, 0));
    // vec4 surfaceToLightVector = normalize(worldView * passPos);
    // fragColour += diffuseColour * (surfaceToLightVector * vec4(passNormal, 0));

    fragColour *= texture(texMap, passUV);

    normalize(fragColour);
}
