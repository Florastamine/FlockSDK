#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"

varying vec2 vScreenPos;
uniform float vx_offset;

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
  vec3 pixcol = texture2D(sDiffMap, uv).rgb;
  vec3 colors[3];
  colors[0] = vec3(0.,0.,1.);
  colors[1] = vec3(1.,1.,0.);
  colors[2] = vec3(1.,0.,0.);
  float lum = (pixcol.r+pixcol.g+pixcol.b)/3.;
  int ix = (lum < 0.5)? 0:1;
  tc = mix(colors[ix],colors[ix+1],(lum-float(ix)*0.5)/0.5);

  gl_FragColor = vec4(tc, 1.0);    
}
