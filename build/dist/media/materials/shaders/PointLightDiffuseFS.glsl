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

// Phong reflection model without specularity.
vec4 pointLightDiffuse(vec3 posV,
                       vec3 normalV,
                       vec3 surfaceToLightV,
                       vec4 attenuationCoeff,
                       vec3 diffuseColour)
{
    float surfaceToLightDistance = length(surfaceToLightV);
    float cosAngleLightNormal = dot(normalV, normalize(surfaceToLightV));

    vec3 colour = vec3(0);
    
    if (cosAngleLightNormal > 0.0 && surfaceToLightDistance <= attenuationCoeff.x)
    {
        float attenuation = attenuationCoeff.y + attenuationCoeff.z * surfaceToLightDistance + attenuationCoeff.w * pow(surfaceToLightDistance, 2);
        float diffuseIntensity = 1.0 / attenuation;
        colour += diffuseColour * cosAngleLightNormal * diffuseIntensity;
    }
    
    return vec4(colour, 1);
}


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
