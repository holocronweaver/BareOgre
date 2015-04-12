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

vec2 generateTexCoord_EnvMap_Sphere(mat4 worldview,
                                    mat4 worldviewIT,
                                    vec4 surfacePos,
                                    vec3 normal)
{
    vec3 eyeDir = normalize(worldview * surfacePos).xyz;
    vec3 normalI = normalize((worldviewIT * vec4(normal, 0.0)).xyz);
    vec3 r = reflect(eyeDir, normalI);
    float two_p = 2.0 * sqrt(r.x * r.x +  r.y * r.y + (r.z + 1) * (r.z + 1));
    return vec2(0.5 + r.x / two_p, 0.5 - r.y / two_p);
}

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
