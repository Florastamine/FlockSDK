#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"

#ifdef DIRLIGHT
    varying vec2 vScreenPos;
#else
    varying vec4 vScreenPos;
#endif
varying vec3 vFarRay;
#ifdef ORTHO
    varying vec3 vNearRay;
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    #ifdef DIRLIGHT
        vScreenPos = GetScreenPosPreDiv(gl_Position);
        vFarRay = GetFarRay(gl_Position);
        #ifdef ORTHO
            vNearRay = GetNearRay(gl_Position);
        #endif
    #else
        vScreenPos = GetScreenPos(gl_Position);
        vFarRay = GetFarRay(gl_Position) * gl_Position.w;
        #ifdef ORTHO
            vNearRay = GetNearRay(gl_Position) * gl_Position.w;
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
    // If rendering a directional light quad, optimize out the w divide
    #ifdef DIRLIGHT
        #ifdef HWDEPTH
            float depth = ReconstructDepth(texture2D(sDepthBuffer, vScreenPos).r);
        #else
            float depth = DecodeDepth(texture2D(sDepthBuffer, vScreenPos).rgb);
        #endif
        #ifdef ORTHO
            vec3 worldPos = mix(vNearRay, vFarRay, depth);
        #else
            vec3 worldPos = vFarRay * depth;
        #endif
        vec4 albedoInput = texture2D(sAlbedoBuffer, vScreenPos);
        vec4 normalInput = texture2D(sNormalBuffer, vScreenPos);
    #else
        #ifdef HWDEPTH
            float depth = ReconstructDepth(texture2DProj(sDepthBuffer, vScreenPos).r);
        #else
            float depth = DecodeDepth(texture2DProj(sDepthBuffer, vScreenPos).rgb);
        #endif
        #ifdef ORTHO
            vec3 worldPos = mix(vNearRay, vFarRay, depth) / vScreenPos.w;
        #else
            vec3 worldPos = vFarRay * depth / vScreenPos.w;
        #endif
        vec4 albedoInput = texture2DProj(sAlbedoBuffer, vScreenPos);
        vec4 normalInput = texture2DProj(sNormalBuffer, vScreenPos);
    #endif
    
    // Position acquired via near/far ray is relative to camera. Bring position to world space
vec3 eyeVec = -worldPos;
worldPos += cCameraPosPS;

vec3 normal = normalize(normalInput.rgb * 2.0 - 1.0);
vec4 projWorldPos = vec4(worldPos, 1.0);
vec3 lightColor;
vec3 lightDir;

float diff = GetDiffuse(normal, worldPos, lightDir);

#ifdef SHADOW
    diff *= GetShadowDeferred(projWorldPos, normal, depth);
    #ifdef VOLUMETRICLIGHT
        highp vec3 rayDirection = normalize(worldPos-cCameraPosPS);
        float accumFog = 0.0;
        const int NB_STEPS = 15;
        #ifdef DIRLIGHT
            float ditherValue = rand(vScreenPos);
        #else
            float ditherValue = rand(vScreenPos.xy/vScreenPos.w);
        #endif
        for (int n = 0; n < NB_STEPS; n++) {
            highp vec4 projWorldPosSpace = vec4(cCameraPosPS+(float(n)+ditherValue)*(worldPos-cCameraPosPS)/float(NB_STEPS), 1.0);
            accumFog += GetShadowDeferred(projWorldPosSpace, vec3(0.0), float(n)*depth/float(NB_STEPS))*ComputeScattering(dot(rayDirection, lightDir));
        }
        accumFog /= float(NB_STEPS);
    #endif
#endif

#if defined(SPOTLIGHT)
    vec4 spotPos = projWorldPos * cLightMatricesPS[0];
    lightColor = spotPos.w > 0.0 ? texture2DProj(sLightSpotMap, spotPos).rgb * cLightColor.rgb : vec3(0.0);
#elif defined(CUBEMASK)
    mat3 lightVecRot = mat3(cLightMatricesPS[0][0].xyz, cLightMatricesPS[0][1].xyz, cLightMatricesPS[0][2].xyz);
    lightColor = textureCube(sLightCubeMap, (worldPos - cLightPosPS.xyz) * lightVecRot).rgb * cLightColor.rgb;
#else
    lightColor = cLightColor.rgb;
#endif

#ifdef SPECULAR
    float spec = GetSpecular(normal, eyeVec, lightDir, normalInput.a * 255.0);
    vec3 finalColor = diff * lightColor * (albedoInput.rgb + spec * cLightColor.a * albedoInput.aaa);
#else
    vec3 finalColor = diff * lightColor * albedoInput.rgb;
#endif

float finalAlpha = 0.0;
#ifdef SHADOW
    #ifdef VOLUMETRICLIGHT
        finalAlpha = accumFog;
    #endif
#endif

gl_FragColor = vec4(finalColor, finalAlpha);
}
