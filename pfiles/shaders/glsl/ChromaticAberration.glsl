// Contains modifications by Florastamine (https://github.com/Florastamine) - 
// Adds two more uniform floats for screen width and height for easier customization through the XML interface. 

#include "Uniforms.glsl"
#include "Samplers.glsl"
#include "Transform.glsl"
#include "ScreenPos.glsl"
#include "Lighting.glsl"

varying vec2 vScreenPos;

uniform float cScreenWidth;
uniform float cScreenHeight;

const float max_distort = 2.2;
const int num_iter = 12;
const float reci_num_iter_f = 1.0 / float(num_iter);

void VS()
{
  mat4 modelMatrix = iModelMatrix;
  vec3 worldPos = GetWorldPos(modelMatrix);
  gl_Position = GetClipPos(worldPos);
  vScreenPos = GetScreenPosPreDiv(gl_Position);
}

vec2 barrelDistortion(vec2 coord, float amt)
{
  vec2 cc = coord - 0.5;
  float dist = dot(cc, cc);
  return coord + cc * dist * amt;
}

float sat( float t ) { return clamp( t, 0.0, 1.0 ); }
float linterp( float t ) { return sat( 1.0 - abs( 2.0*t - 1.0 ) ); }
float remap( float t, float a, float b ) { return sat( (t - a) / (b - a) ); }

vec4 spectrum_offset( float t ) {
  vec4 ret;
  float lo = step(t,0.5);
  float hi = 1.0-lo;
  float w = linterp( remap( t, 1.0/6.0, 5.0/6.0 ) );
  ret = vec4(lo,1.0,hi, 1.) * vec4(1.0-w, w, 1.0-w, 1.);

  return pow( ret, vec4(1.0/2.2) );
}

void PS()
{
  vec2 uv = (gl_FragCoord.xy / vec2(cScreenWidth, cScreenHeight) * .5) + .25;

  vec4 sumcol = vec4(0.0);
  vec4 sumw = vec4(0.0);	
  for ( int i=0; i<num_iter;++i )
  {
    float t = float(i) * reci_num_iter_f;
    vec4 w = spectrum_offset( t );
    sumw += w;
    sumcol += w * texture2D( sDiffMap, barrelDistortion(uv, .6 * max_distort*t ) );
  }
		
  gl_FragColor = sumcol / sumw;
}
