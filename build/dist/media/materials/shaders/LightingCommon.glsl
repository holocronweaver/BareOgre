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

// void FFP_Light_Point_Diffuse(in mat4 mWorldView, 
//                              in vec4 vPos,
//                              in mat4 mWorldViewIT, 
//                              in vec3 vNormal,
//                              in vec3 vLightPosView,
//                              in vec4 vAttParams,
//                              in vec3 vDiffuseColour, 
//                              in vec3 vBaseColour, 
//                              out vec3 vOut)
// {
// 	vOut = vBaseColour;		

// 	vec3 vViewPos    = (mWorldView * vPos).xyz;
// 	vec3 vLightView  = vLightPosView - vViewPos;
// 	float fLightD      = length(vLightView);
// 	vec3 vNormalView = normalize((mWorldViewIT * vec4(vNormal.xyz, 1.0)).xyz); 
// 	float nDotL        = dot(vNormalView, normalize(vLightView));
	
// 	if (nDotL > 0.0 && fLightD <= vAttParams.x)
// 	{
// 		float fAtten	   = 1.0 / (vAttParams.y + vAttParams.z*fLightD + vAttParams.w*fLightD*fLightD);
			
// 		vOut += vDiffuseColour * nDotL * fAtten;
// 	}		
// }
