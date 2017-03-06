// Contains modifications by Florastamine (https://github.com/Florastamine) - 
// Removal of OpenGL ES-specific code. Replaced the Pi constant with the one from Constants.glsl

#include "Constants.glsl" 
#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

varying vec2 vScreenPos;

#ifdef COMPILEPS
    uniform float cOffset;
    uniform float cFrequency;
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
}

void PS()
{
    float global_pos = (vScreenPos.y + cOffset) * cFrequency;
    float wave_pos = cos((fract(global_pos) - 0.5) * M_PI);
    vec4 pel = texture2D(sDiffMap, vScreenPos);

    gl_FragColor = mix(vec4(0,0,0,0), pel, wave_pos);
}
