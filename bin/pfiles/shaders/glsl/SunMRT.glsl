#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

varying vec2 vTexCoord;
varying vec2 vScreenPos;

#ifdef COMPILEPS
    uniform float cExposure;
    uniform float cDecay;
    uniform float cDensity;
    uniform float cWeight;
    uniform vec2 cLightPositionOnScreen;
    uniform vec2 cMRTInvSize;

    const int NUM_SAMPLES = 100;
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vTexCoord = GetQuadTexCoord(gl_Position);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
}

#ifdef COMPILEPS
    void PS()
    {
        vec2 deltaTexCoords = vec2(vScreenPos - cLightPositionOnScreen);
        deltaTexCoords *= 1.0 / float(NUM_SAMPLES) * cDensity;
        float illyminationDecay = 1.0;
        vec2 texCoords = vScreenPos;
        vec4 color = vec4(0.0);
        for (int i = 0; i < NUM_SAMPLES; i++) 
        {
                texCoords -= deltaTexCoords;
                vec4 sample = texture2D(sDiffMap, texCoords); 
                sample *= illyminationDecay * cWeight;
                color += sample;
                illyminationDecay *= cDecay;    
        }
        
        gl_FragColor = vec4(vec3(color * cExposure), 1.0);
    }
#endif
