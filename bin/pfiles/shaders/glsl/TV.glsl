// Contains modifications by Florastamine (https://github.com/Florastamine) - 
// Replaced Pi and Pi/2 constants with the ones from Constants.glsl. Disabled the fisheye filter. 
// The fisheye filter can be unlocked by removing the WITH_FISHEYE switch from the shader's XML interface. 

#include "Constants.glsl" 
#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

varying vec2 vScreenPos;
precision mediump float; 
uniform float noise_seed; 

#ifdef WITH_FISHEYE 
  const float mScale = 0.5; 
  const float alpha = mScale * 2.0 + 0.75; 
  vec2 scale = vec2(1.0, 1.0); 
  float bound2 = 0.25 * (scale[0] * scale[0] + scale[1] * scale[1]); 
  float bound = sqrt(bound2); 
  float radius = 1.15 * bound; 
  float radius2 = radius * radius; 
  float max_radian = 0.5 * M_PI - atan(alpha / bound * sqrt(radius2 - bound2)); 
  float factor = bound / max_radian; 
#endif

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
}

float rand(vec2 loc)
{
  vec2 scale = vec2(1.0, 1.0); 
  float theta1 = dot(loc, vec2(0.9898, 0.233)); 
  float theta2 = dot(loc, vec2(12.0, 78.0)); 
  float value = cos(theta1) * sin(theta2) + sin(theta1) * cos(theta2); 
  float temp = mod(197.0 * value, 1.0) + value; 
  float part1 = mod(220.0 * temp, 1.0) + temp; 
  float part2 = value * 0.5453; 
  float part3 = cos(theta1 + theta2) * 0.43758; 
  return fract(part1 + part2 + part3); 
} 

void PS()  
{ 
  #ifdef WITH_FISHEYE 
    scale = vec2(1.0, 1.0); 
    vec2 coord = vScreenPos - vec2(0.5, 0.5); 
    float dist = length(coord * scale); 
    float radian = M_PI_2 - atan(alpha * sqrt(radius2 - dist * dist), dist); 
    float scalar = radian * factor / dist; 
    vec2 new_coord = coord * scalar + vec2(0.5, 0.5); 
    gl_FragColor = texture2D(sDiffMap, new_coord);
  #else
    gl_FragColor = texture2D(sDiffMap, vScreenPos);
  #endif
     
  // Make some noise!!! ;-)
  vec2 vec_offset = vec2(noise_seed, 0); 
  float brightness = rand(vScreenPos + vec_offset) + .5;   
  gl_FragColor = brightness * gl_FragColor;     
  // B&W
  float y = dot(gl_FragColor, vec4(0.299, 0.587, 0.114, 0)); 
  gl_FragColor = vec4(y, y, y, gl_FragColor.a);   
}
