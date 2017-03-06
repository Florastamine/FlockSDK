#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"

varying vec2 vScreenPos;
uniform sampler2D sceneTex; 
uniform float vx_offset;

#ifdef COMPILEPS
    uniform float cHatchYOffset;
    uniform float cLumThreshold1;
    uniform float cLumThreshold2;
    uniform float cLumThreshold3;
    uniform float cLumThreshold4;
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
  vec2 uv = vScreenPos.xy;
  
  vec3 tc = vec3(1.0, 0.0, 0.0);
  if (uv.x > (vx_offset-0.005))
  {
    float lum = length(texture2D(sceneTex, uv).rgb);
    tc = vec3(1.0, 1.0, 1.0);
  
    if (lum > cLumThreshold1) 
    {
      if (mod(gl_FragCoord.x + gl_FragCoord.y, 10.0) == 0.0) 
        tc = vec3(0.0, 0.0, 0.0);
    }  
  
    if (lum > cLumThreshold2) 
    {
      if (mod(gl_FragCoord.x - gl_FragCoord.y, 10.0) == 0.0) 
        tc = vec3(0.0, 0.0, 0.0);
    }  
  
    if (lum > cLumThreshold3) 
    {
      if (mod(gl_FragCoord.x + gl_FragCoord.y - cHatchYOffset, 10.0) == 0.0) 
        tc = vec3(0.0, 0.0, 0.0);
    }  
  
    if (lum > cLumThreshold4) 
    {
      if (mod(gl_FragCoord.x - gl_FragCoord.y - cHatchYOffset, 10.0) == 0.0) 
        tc = vec3(0.0, 0.0, 0.0);
    }
  }
  else if (uv.x<=(vx_offset+0.005))
  {
    tc = texture2D(sceneTex, uv).rgb;
  }
  
  gl_FragColor = vec4(tc, 1.0);
}
