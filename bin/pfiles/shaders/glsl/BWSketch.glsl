#include "Uniforms.glsl" 
#include "Samplers.glsl" 
#include "Transform.glsl" 
#include "ScreenPos.glsl" 

varying vec2 vScreenPos; 

#ifdef COMPILEPS
	uniform float cBWSketchScreenWidth;
	uniform float cBWSketchScreenHeight;

    uniform float cBWSketchdt;
#endif 

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
}

#ifdef COMPILEPS
    float rand(float x)
    {
        return fract(sin(x) * 43758.5453);
    }

    float triangle(float x)
    {
        return abs(1.0 - mod(abs(x), 2.0)) * 2.0 - 1.0;
    }

    void PS() 
    {
        vec2 screenSize = vec2(cBWSketchScreenWidth, cBWSketchScreenHeight); 
        vec2 uv = gl_FragCoord.xy / screenSize.xy;
        
        float time = floor(cBWSketchdt * 16.0) / 16.0;        
        
        // pixel position
        vec2 p = uv;	
        p += vec2(triangle(p.y * rand(time) * 4.0) * rand(time * 1.9) * 0.015,
                triangle(p.x * rand(time * 3.4) * 4.0) * rand(time * 2.1) * 0.015);
        p += vec2(rand(p.x * 3.1 + p.y * 8.7) * 0.01,
                rand(p.x * 1.1 + p.y * 6.7) * 0.01);
        
        #ifdef PERFORM_DISTORT
            vec2 blurredUV = vec2(p.x+0.003,p.y+0.003);
            vec4 baseColor = vec4(texture(sDiffMap, blurredUV).rgb,1.);
        #else
            vec4 baseColor = vec4(texture(sDiffMap, uv).rgb,1.);
        #endif
        
        vec4 edges = 1.0 - (baseColor / vec4(texture(sDiffMap,p).rgb, 1.));
        
        #ifdef PERFORM_INVERT
            baseColor.rgb = vec3(baseColor.r);    
            gl_FragColor = baseColor / vec4(length(edges));
        #else
            gl_FragColor = vec4(length(edges));
        #endif
    }
#endif 
