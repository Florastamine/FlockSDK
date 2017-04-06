#include "Uniforms.glsl" 
#include "Samplers.glsl" 
#include "Transform.glsl" 
#include "ScreenPos.glsl" 

varying vec2 vScreenPos; 

#ifdef COMPILEPS
    #define ErrorPeriod 30.0
    #define ErrorRange 0.003

    uniform float cPencilScreenWidth;
    uniform float cPencilScreenHeight; 
    uniform float cNoiseAmount;
    uniform vec4 cEdgeColor; 
    uniform vec4 cBackgroundColor;

    uniform float cPencildt; 

    // Reference: https://www.shadertoy.com/view/MsSGD1
    float triangle(float x)
    {
        return abs(1.0 - mod(abs(x), 2.0)) * 2.0 - 1.0;
    }

    float rand(float x)
    {
        return fract(sin(x) * 43758.5453);
    }
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
        float time = floor(cPencildt * 16.0) / 16.0;
        vec2 uv = gl_FragCoord.xy / vec2(cPencilScreenWidth, cPencilScreenHeight).xy;
        uv += vec2(triangle(uv.y * rand(time) * 1.0) * rand(time * 1.9) * 0.005,
                triangle(uv.x * rand(time * 3.4) * 1.0) * rand(time * 2.1) * 0.005);
        
        float noise = (texture(sNormalMap, uv * 0.5).r - 0.5) * cNoiseAmount;
        vec2 uvs[3];
        uvs[0] = uv + vec2(ErrorRange * sin(ErrorPeriod * uv.y + 0.0) + noise, ErrorRange * sin(ErrorPeriod * uv.x + 0.0) + noise);
        uvs[1] = uv + vec2(ErrorRange * sin(ErrorPeriod * uv.y + 1.047) + noise, ErrorRange * sin(ErrorPeriod * uv.x + 3.142) + noise);
        uvs[2] = uv + vec2(ErrorRange * sin(ErrorPeriod * uv.y + 2.094) + noise, ErrorRange * sin(ErrorPeriod * uv.x + 1.571) + noise);
        
        float edge = texture(sDiffMap, uvs[0]).r * texture(sDiffMap, uvs[1]).r * texture(sDiffMap, uvs[2]).r;
        float diffuse = texture(sDiffMap, uv).g;
        
        float w = fwidth(diffuse) * 2.0;
        vec4 mCol = mix(cBackgroundColor * 0.5, cBackgroundColor, mix(0.0, 1.0, smoothstep(-w, w, diffuse - 0.3)));
        gl_FragColor = mix(cEdgeColor, mCol, edge);
    }
#endif 
