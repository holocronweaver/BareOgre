#version 150

in vec4 passPos;
in vec3 passNormal;
in vec2 passUV;

out vec4 fragColour;

uniform mat4 worldview;
uniform mat4 worldviewIT;

uniform sampler2D sphereMap;

uniform vec4 sceneColour;
uniform vec4 diffuseColour;

uniform vec4 surfaceToLightVector;

void main()
{
    fragColour = sceneColour;
    fragColour += diffuseColour * (normalize(surfaceToLightVector) * vec4(passNormal, 0));
    vec2 sphereMapCoord = generateTexCoord_EnvMap_Sphere(worldview,
                                                         worldviewIT,
                                                         passPos,
                                                         passNormal);
    fragColour += texture(sphereMap, sphereMapCoord);
    normalize(fragColour);
}
