// Contains modifications by Florastamine (https://github.com/Florastamine) - 
// Adds two more uniform floats for screen width and height. 

#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"

varying vec2 vScreenPos;
varying vec2 vTexCoord;
varying vec4 vColor;

uniform float cScreenWidth; 
uniform float cScreenHeight; 

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
    vTexCoord = GetQuadTexCoord(gl_Position);
    vColor = iColor; 
}

void PS()
{
  vec2 onePixel = vec2(1.0 / cScreenWidth, 1.0 / cScreenHeight);
  vec2 texCoord = vScreenPos;
  vec4 color1;
  color1.rgb = vec3(0.5);
  color1 -= texture2D(sDiffMap, texCoord - onePixel) * 5.0;
  color1 += texture2D(sDiffMap, texCoord + onePixel) * 5.0;
  color1.rgb = vec3((color1.r + color1.g + color1.b) / 3.0);
  gl_FragColor = vec4(color1.rgb, 1);
}
