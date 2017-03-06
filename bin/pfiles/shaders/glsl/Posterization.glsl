#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"

varying vec2 vScreenPos;

#ifdef COMPILEPS
    uniform float cGamma;
    uniform float cNumColors;
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
  vec3 c = texture2D(sDiffMap, vScreenPos.xy).rgb;
  c = pow(c, vec3(cGamma, cGamma, cGamma));
  c = c * cNumColors;
  c = floor(c);
  c = c / cNumColors;
  c = pow(c, vec3(1.0/cGamma));
  gl_FragColor = vec4(c, 1.0);
}
