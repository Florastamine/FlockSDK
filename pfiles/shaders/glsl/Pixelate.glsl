// Contains modifications by Florastamine (https://github.com/Florastamine) - 
// Adds two more uniform floats for screen width and height, along with the removal of rt_w and rt_h. 

#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

varying vec2 vScreenPos;
uniform sampler2D sceneTex; 

#ifdef COMPILEPS
  uniform float cVXOffset;
  uniform float cPixelWidth;
  uniform float cPixelHeight;
  uniform float cScreenWidth;
  uniform float cScreenHeight;
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
  if (uv.x < (cVXOffset-0.005))
  {
    float dx = cPixelWidth * (1.0 / float(cScreenWidth)); // Seems like GLSL doesn't allow C-style casting, so we 
                                                          // have to use the constructor-like syntax for casting 
                                                          // stuff - that is, no "(float) i", just "float(i)" is allowed.
    float dy = cPixelHeight * (1.0 / float(cScreenHeight));
    
    vec2 coord = vec2(dx*floor(uv.x/dx), dy*floor(uv.y/dy));
    tc = texture2D(sceneTex, coord).rgb;
  }
  else if (uv.x>=(cVXOffset+0.005)) 
    tc = texture2D(sceneTex, uv).rgb;

  gl_FragColor = vec4(tc, 1.0);
}
