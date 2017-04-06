#include "Uniforms.glsl" 
#include "Samplers.glsl" 
#include "Transform.glsl" 
#include "ScreenPos.glsl" 

varying vec2 vScreenPos; 

#ifdef COMPILEPS
	uniform float cSTScreenWidth;
	uniform float cSTScreenHeight;
#endif 

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
}

#ifdef COMPILEPS
    void PS() 
    {
        vec2 screenSize = vec2(cSTScreenWidth, cSTScreenHeight); 
        vec2 st = gl_FragCoord.xy / screenSize.xy;
        
        vec3 c = texture(sDiffMap, st).rgb;
        c = sin(st.x * 10.0 + c.r * 40.0) > 0.0 ? vec3(1.0) : vec3(0.0);
            
        gl_FragColor = vec4(c, 1.0);    
    }
#endif 
