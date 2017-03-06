// Contains modifications by Florastamine (https://github.com/Florastamine) - 
// Removal of OpenGL-specific code. 

#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

varying vec2 vScreenPos;

#ifdef COMPILEPS
  uniform float cSharpenFactor;  
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
   vec4 Color = texture2D( sDiffMap, vScreenPos.xy );   

   Color -= texture2D( sDiffMap, vScreenPos+0.0001)*cSharpenFactor;
   Color += texture2D( sDiffMap, vScreenPos-0.0001)*cSharpenFactor;
   Color.a = 1.0;
   
   gl_FragColor = Color;
}
