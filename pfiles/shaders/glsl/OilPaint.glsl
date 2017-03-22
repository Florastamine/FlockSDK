#include "Uniforms.glsl" 
#include "Samplers.glsl" 
#include "Transform.glsl" 
#include "ScreenPos.glsl" 

varying vec2 vScreenPos; 

#ifdef COMPILEPS 
  uniform float cOilRadius; 
  uniform float cOilScreenWidth;
  uniform float cOilScreenHeight; 
#endif 

void VS()
{
    mat4 modelMatrix = iModelMatrix;
    vec3 worldPos = GetWorldPos(modelMatrix);
    gl_Position = GetClipPos(worldPos);
    vScreenPos = GetScreenPosPreDiv(gl_Position);
}

#ifdef COMPILEPS 
    void PS() { 
        vec2 src = vec2(cOilScreenWidth, cOilScreenHeight); 
        vec2 src_size = vec2(1.0 / src.x, 1.0 / src.y); 
        vec2 uv = gl_FragCoord.xy / src.xy;
        float n = float((int(cOilRadius) + 1) * (int(cOilRadius) + 1));
        int i; 
        int j;
        vec3 m0 = vec3(0.0); vec3 m1 = vec3(0.0); vec3 m2 = vec3(0.0); vec3 m3 = vec3(0.0);
        vec3 s0 = vec3(0.0); vec3 s1 = vec3(0.0); vec3 s2 = vec3(0.0); vec3 s3 = vec3(0.0);
        vec3 c;

        for (int j = -int(cOilRadius); j <= 0; ++j)  {
            for (int i = -int(cOilRadius); i <= 0; ++i)  {
                c = texture2D(sDiffMap, uv + vec2(i,j) * src_size).rgb;
                m0 += c;
                s0 += c * c;
            }
        }

        for (int j = -int(cOilRadius); j <= 0; ++j)  {
            for (int i = 0; i <= int(cOilRadius); ++i)  {
                c = texture2D(sDiffMap, uv + vec2(i,j) * src_size).rgb;
                m1 += c;
                s1 += c * c;
            }
        }

        for (int j = 0; j <= int(cOilRadius); ++j)  {
            for (int i = 0; i <= int(cOilRadius); ++i)  {
                c = texture2D(sDiffMap, uv + vec2(i,j) * src_size).rgb;
                m2 += c;
                s2 += c * c;
            }
        }

        for (int j = 0; j <= int(cOilRadius); ++j)  {
            for (int i = -int(cOilRadius); i <= 0; ++i)  {
                c = texture2D(sDiffMap, uv + vec2(i,j) * src_size).rgb;
                m3 += c;
                s3 += c * c;
            }
        }

        float min_sigma2 = 1e+2;
        m0 /= n;
        s0 = abs(s0 / n - m0 * m0);

        float sigma2 = s0.r + s0.g + s0.b;
        if (sigma2 < min_sigma2) {
            min_sigma2 = sigma2;
            gl_FragColor = vec4(m0, 1.0);
        }

        m1 /= n;
        s1 = abs(s1 / n - m1 * m1);

        sigma2 = s1.r + s1.g + s1.b;
        if (sigma2 < min_sigma2) {
            min_sigma2 = sigma2;
            gl_FragColor = vec4(m1, 1.0);
        }

        m2 /= n;
        s2 = abs(s2 / n - m2 * m2);

        sigma2 = s2.r + s2.g + s2.b;
        if (sigma2 < min_sigma2) {
            min_sigma2 = sigma2;
            gl_FragColor = vec4(m2, 1.0);
        }

        m3 /= n;
        s3 = abs(s3 / n - m3 * m3);

        sigma2 = s3.r + s3.g + s3.b;
        if (sigma2 < min_sigma2) {
            min_sigma2 = sigma2;
            gl_FragColor = vec4(m3, 1.0);
        }
    }
#endif 
