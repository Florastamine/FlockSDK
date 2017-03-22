#include "Uniforms.glsl" 
#include "Samplers.glsl" 
#include "Transform.glsl" 
#include "ScreenPos.glsl" 

varying vec2 vScreenPos; 

float d; 

#ifdef COMPILEPS 
    uniform float cED_R; 
    uniform float cED_G; 
    uniform float cED_B; 

    uniform float cED_ScreenWidth;
    uniform float cED_ScreenHeight; 
#endif 

#ifdef COMPILEPS 
    float __lookup(vec2 p, float dx, float dy)
    {
        vec2 uv = (p.xy + vec2(dx * d, dy * d)) / vec2(cED_ScreenWidth, cED_ScreenHeight); 
        vec4 c = texture2D(sDiffMap, uv.xy); 

        return 0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b;
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
        d = sin(1.0 * 5.0) * 0.5 + 1.5; 
        vec2 p = gl_FragCoord.xy; 
        
        // simple sobel edge detection
        float gx = 0.0;
        gx += -1.0 * __lookup(p, -1.0, -1.0);
        gx += -2.0 * __lookup(p, -1.0,  0.0);
        gx += -1.0 * __lookup(p, -1.0,  1.0);
        gx +=  1.0 * __lookup(p,  1.0, -1.0);
        gx +=  2.0 * __lookup(p,  1.0,  0.0);
        gx +=  1.0 * __lookup(p,  1.0,  1.0);
        
        float gy = 0.0;
        gy += -1.0 * __lookup(p, -1.0, -1.0);
        gy += -2.0 * __lookup(p,  0.0, -1.0);
        gy += -1.0 * __lookup(p,  1.0, -1.0);
        gy +=  1.0 * __lookup(p, -1.0,  1.0);
        gy +=  2.0 * __lookup(p,  0.0,  1.0);
        gy +=  1.0 * __lookup(p,  1.0,  1.0);
        
        // hack: use g^2 to conceal noise in the video
        float g = gx*gx + gy*gy;
        float g2 = g * (sin(1.0) / 2.0 + 0.5);
        
        vec4 col = texture2D(sDiffMap, p / vec2(cED_ScreenWidth, cED_ScreenHeight)); 
        col += vec4(0.0, g, g2, 1.0);
        
        gl_FragColor = col;
    }
#endif 
