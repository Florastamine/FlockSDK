#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"
#include "Fog.glsl"

#ifdef NORMALMAP
    varying vec4 vTexCoord;
    varying vec4 vTangent;
#else
    varying vec2 vTexCoord;
#endif
varying vec3 vNormal;
varying vec4 vWorldPos;
#ifdef VERTEXCOLOR
    varying vec4 vColor;
#endif
#ifdef PERPIXEL
    #ifdef SHADOW
            varying vec4 vShadowPos[NUMCASCADES];
            #ifdef VOLUMETRICLIGHT
                varying vec2 vScreenPos;
                varying vec4 vShadowPosCamera[NUMCASCADES];
                varying vec4 vShadowPosTarget[NUMCASCADES];
            #endif
    #endif
    #ifdef SPOTLIGHT
        varying vec4 vSpotPos;
    #endif
    #ifdef POINTLIGHT
        varying vec3 vCubeMaskVec;
    #endif
#else
    varying vec3 vVertexLight;
    varying vec4 vScreenPos;
    #ifdef ENVCUBEMAP
        varying vec3 vReflectionVec;
    #endif
    #if defined(LIGHTMAP) || defined(AO)
        varying vec2 vTexCoord2;
    #endif
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vNormal = GetWorldNormal(modelMatrix);
    vWorldPos = vec4(worldPos, GetDepth(gl_Position));

    #ifdef VERTEXCOLOR
    vColor = iColor;
#endif

#ifdef NORMALMAP
    vec3 tangent = GetWorldTangent(modelMatrix);
    vec3 bitangent = cross(tangent, vNormal) * iTangent.w;
    vTexCoord = vec4(GetTexCoord(iTexCoord), bitangent.xy);
    vTangent = vec4(tangent, bitangent.z);
#else
    vTexCoord = GetTexCoord(iTexCoord);
#endif

#ifdef PERPIXEL
    // Per-pixel forward lighting
    vec4 projWorldPos = vec4(worldPos, 1.0);

    #ifdef SHADOW
        // Shadow projection: transform from world space to shadow space
        for (int i = 0; i < NUMCASCADES; i++)
            vShadowPos[i] = GetShadowPos(i, vNormal, projWorldPos);
        #ifdef VOLUMETRICLIGHT
            // Shadow projection: transform from world space to shadow space
            for (int i = 0; i < NUMCASCADES; i++)
                vShadowPosCamera[i] = GetShadowPos(i, vec3(0), vec4(cCameraPos, 1.0));
            // Shadow projection: transform from world space to shadow space
            for (int i = 0; i < NUMCASCADES; i++)
                vShadowPosTarget[i] = GetShadowPos(i, vec3(0), projWorldPos);
            vScreenPos = GetScreenPosPreDiv(gl_Position);
        #endif
    #endif

    #ifdef SPOTLIGHT
        // Spotlight projection: transform from world space to projector texture coordinates
        vSpotPos = projWorldPos * cLightMatrices[0];
    #endif

    #ifdef POINTLIGHT
        vCubeMaskVec = (worldPos - cLightPos.xyz) * mat3(cLightMatrices[0][0].xyz, cLightMatrices[0][1].xyz, cLightMatrices[0][2].xyz);
    #endif
#else
    // Ambient & per-vertex lighting
    #if defined(LIGHTMAP) || defined(AO)
        // If using lightmap, disregard zone ambient light
        // If using AO, calculate ambient in the PS
        vVertexLight = vec3(0.0, 0.0, 0.0);
        vTexCoord2 = iTexCoord1;
    #else
        vVertexLight = GetAmbient(GetZonePos(worldPos));
    #endif
    
    #ifdef NUMVERTEXLIGHTS
        for (int i = 0; i < NUMVERTEXLIGHTS; ++i)
            vVertexLight += GetVertexLight(i, worldPos, vNormal) * cVertexLights[i * 3].rgb;
    #endif
    
    vScreenPos = GetScreenPos(gl_Position);

    #ifdef ENVCUBEMAP
        vReflectionVec = worldPos - cCameraPos;
    #endif
#endif
}

#ifdef SHADOW
    #ifdef VOLUMETRICLIGHT
        float ComputeScattering(float lightDotView)
        {
            const float G_SCATTERING = 0.5;
            const float PI = 3.14159265358979323846;
            float result = 1.0 - G_SCATTERING * G_SCATTERING;
            result /= (4.0 * PI * pow(1.0 + G_SCATTERING * G_SCATTERING - (2.0 * G_SCATTERING) * lightDotView, 1.5));
            return result;
        }
        float rand(highp vec2 co){
        return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
    }
#endif
#endif
void PS()
{
    // Get material diffuse albedo
    #ifdef DIFFMAP
        vec4 diffInput = texture2D(sDiffMap, vTexCoord.xy);
        #ifdef ALPHAMASK
            if (diffInput.a < 0.5)
                discard;
        #endif
        vec4 diffColor = cMatDiffColor * diffInput;
    #else
        vec4 diffColor = cMatDiffColor;
    #endif
    
    #ifdef VERTEXCOLOR
    diffColor *= vColor;
#endif

// Get material specular albedo
#ifdef SPECMAP
    vec3 specColor = cMatSpecColor.rgb * texture2D(sSpecMap, vTexCoord.xy).rgb;
#else
    vec3 specColor = cMatSpecColor.rgb;
#endif

// Get normal
#ifdef NORMALMAP
    mat3 tbn = mat3(vTangent.xyz, vec3(vTexCoord.zw, vTangent.w), vNormal);
    vec3 normal = normalize(tbn * DecodeNormal(texture2D(sNormalMap, vTexCoord.xy)));
#else
    vec3 normal = normalize(vNormal);
#endif

// Get fog factor
#ifdef HEIGHTFOG
    float fogFactor = GetHeightFogFactor(vWorldPos.w, vWorldPos.y);
#else
    float fogFactor = GetFogFactor(vWorldPos.w);
#endif

#if defined(PERPIXEL)
    // Per-pixel forward lighting
    vec3 lightColor;
    vec3 lightDir;
    vec3 finalColor;

    float diff = GetDiffuse(normal, vWorldPos.xyz, lightDir);

    #ifdef SHADOW
        diff *= GetShadow(vShadowPos, vWorldPos.w);
        #ifdef VOLUMETRICLIGHT
            highp vec3 rayDirection = normalize(vWorldPos.xyz-cCameraPosPS);
            float accumFog = 0.0;
            const int NB_STEPS = 15;
            float ditherValue = rand(vScreenPos);
            for (int n = 0; n < NB_STEPS; n++) {
                highp vec4 vShadowPosSpace[NUMCASCADES];
                for (int i = 0; i < NUMCASCADES; i++) {
                    vShadowPosSpace[i] = vShadowPosCamera[i]+(float(n)+ditherValue)*(vShadowPosTarget[i]-vShadowPosCamera[i])/float(NB_STEPS);
                }
                accumFog += GetShadow(vShadowPosSpace, float(n)*vWorldPos.w/float(NB_STEPS))*ComputeScattering(dot(rayDirection, lightDir));
            }
            accumFog /= float(NB_STEPS);
        #endif
    #endif

    #if defined(SPOTLIGHT)
        lightColor = vSpotPos.w > 0.0 ? texture2DProj(sLightSpotMap, vSpotPos).rgb * cLightColor.rgb : vec3(0.0, 0.0, 0.0);
    #elif defined(CUBEMASK)
        lightColor = textureCube(sLightCubeMap, vCubeMaskVec).rgb * cLightColor.rgb;
    #else
        lightColor = cLightColor.rgb;
    #endif

    #ifdef SPECULAR
        float spec = GetSpecular(normal, cCameraPosPS - vWorldPos.xyz, lightDir, cMatSpecColor.a);
        finalColor = diff * lightColor * (diffColor.rgb + spec * specColor * cLightColor.a);
    #else
        finalColor = diff * lightColor * diffColor.rgb;
    #endif

    #ifdef AMBIENT
        finalColor += cAmbientColor.rgb * diffColor.rgb;
        finalColor += cMatEmissiveColor;
        finalColor = GetFog(finalColor, fogFactor);
    #else
        finalColor = GetLitFog(finalColor, fogFactor);
    #endif

    #ifndef VOLUMETRICLIGHT
        float finalAlpha = diffColor.a;
    #else
        float finalAlpha = 0.0;
    #endif

    #ifdef SHADOW
        #ifdef VOLUMETRICLIGHT
            finalAlpha = accumFog;
        #endif
    #endif
    gl_FragColor = vec4(finalColor, finalAlpha);
#elif defined(PREPASS)
    // Fill light pre-pass G-Buffer
    float specPower = cMatSpecColor.a / 255.0;

    gl_FragData[0] = vec4(normal * 0.5 + 0.5, specPower);
    gl_FragData[1] = vec4(EncodeDepth(vWorldPos.w), 0.0);
#elif defined(DEFERRED)
    // Fill deferred G-buffer
    float specIntensity = specColor.g;
    float specPower = cMatSpecColor.a / 255.0;

    vec3 finalColor = vVertexLight * diffColor.rgb;
    #ifdef AO
        // If using AO, the vertex light ambient is black, calculate occluded ambient here
        finalColor += texture2D(sEmissiveMap, vTexCoord2).rgb * cAmbientColor.rgb * diffColor.rgb;
    #endif

    #ifdef ENVCUBEMAP
        finalColor += cMatEnvMapColor * textureCube(sEnvCubeMap, reflect(vReflectionVec, normal)).rgb;
    #endif
    #ifdef LIGHTMAP
        finalColor += texture2D(sEmissiveMap, vTexCoord2).rgb * diffColor.rgb;
    #endif
    #ifdef EMISSIVEMAP
        finalColor += cMatEmissiveColor * texture2D(sEmissiveMap, vTexCoord.xy).rgb;
    #else
        finalColor += cMatEmissiveColor;
    #endif

    #ifndef VOLUMETRICLIGHT
        gl_FragData[0] = vec4(GetFog(finalColor, fogFactor), 1.0);
    #else
        gl_FragData[0] = vec4(GetFog(finalColor, fogFactor), 0.0);
    #endif
    gl_FragData[1] = fogFactor * vec4(diffColor.rgb, specIntensity);
    gl_FragData[2] = vec4(normal * 0.5 + 0.5, specPower);
    gl_FragData[3] = vec4(EncodeDepth(vWorldPos.w), 0.0);
#else
    // Ambient & per-vertex lighting
    vec3 finalColor = vVertexLight * diffColor.rgb;
    #ifdef AO
        // If using AO, the vertex light ambient is black, calculate occluded ambient here
        finalColor += texture2D(sEmissiveMap, vTexCoord2).rgb * cAmbientColor.rgb * diffColor.rgb;
    #endif
    
    #ifdef MATERIAL
        // Add light pre-pass accumulation result
        // Lights are accumulated at half intensity. Bring back to full intensity now
        vec4 lightInput = 2.0 * texture2DProj(sLightBuffer, vScreenPos);
        vec3 lightSpecColor = lightInput.a * lightInput.rgb / max(GetIntensity(lightInput.rgb), 0.001);

        finalColor += lightInput.rgb * diffColor.rgb + lightSpecColor * specColor;
    #endif

    #ifdef ENVCUBEMAP
        finalColor += cMatEnvMapColor * textureCube(sEnvCubeMap, reflect(vReflectionVec, normal)).rgb;
    #endif
    #ifdef LIGHTMAP
        finalColor += texture2D(sEmissiveMap, vTexCoord2).rgb * diffColor.rgb;
    #endif
    #ifdef EMISSIVEMAP
        finalColor += cMatEmissiveColor * texture2D(sEmissiveMap, vTexCoord.xy).rgb;
    #else
        finalColor += cMatEmissiveColor;
    #endif

    gl_FragColor = vec4(GetFog(finalColor, fogFactor), diffColor.a);
#endif
}
