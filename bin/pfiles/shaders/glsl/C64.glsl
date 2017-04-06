#include "Uniforms.glsl" 
#include "Samplers.glsl" 
#include "Transform.glsl" 
#include "ScreenPos.glsl" 

varying vec2 vScreenPos; 

#ifdef COMPILEPS
	uniform float cC64ScreenWidth;
	uniform float cC64ScreenHeight;

	float pixelSize = 64.0;

	float find_closest(int x, int y, float c0) {
		int dither[64];

		dither[0] = 0; dither[1] = 32; dither[2] = 8; dither[3] = 40; 
		dither[4] = 2; dither[5] = 32; dither[6] = 10; dither[7] = 42; 
		dither[8] = 48; dither[9] = 16; dither[10] = 56; dither[11] = 24; 
		dither[12] = 50; dither[13] = 18; dither[14] = 58; dither[15] = 26; 
		dither[16] = 12; dither[17] = 44; dither[18] = 4; dither[19] = 36; 
		dither[20] = 14; dither[21] = 46; dither[22] = 6; dither[23] = 38;
		dither[24] = 60; dither[25] = 28; dither[26] = 52; dither[27] = 20; 
		dither[28] = 62; dither[29] = 30; dither[30] = 54; dither[31] = 22; 
		dither[32] = 3; dither[33] = 35; dither[34] = 11; dither[35] = 43; 
		dither[36] = 1; dither[37] = 33; dither[38] = 9; dither[39] = 41; 
		dither[40] = 51; dither[41] = 19; dither[42] = 59; dither[43] = 27; 
		dither[44] = 49; dither[45] = 17; dither[46] = 57; dither[47] = 25; 
		dither[48] = 15; dither[49] = 47; dither[50] = 7; dither[51] = 39; 
		dither[52] = 13; dither[53] = 45; dither[54] = 5; dither[55] = 37; 
		dither[56] = 63; dither[57] = 31; dither[58] = 55; dither[59] = 23; 
		dither[60] = 61; dither[61] = 29; dither[62] = 53; dither[63] = 21;

		float limit = 0.0;
		if(x < 8) {
			int index = x + y*8;
			for (int i = 0; i < 64; ++i) 
				if(index == i) 
					limit = float(dither[i]+1)/64.0; 
		}

		if(c0 < limit)
			return 0.0;
		
		return 1.0;
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
		vec2 screenSize = vec2(cC64ScreenWidth, cC64ScreenHeight); 

		//Pixelate
		vec2 uv = gl_FragCoord.xy / screenSize.xy;
		vec2 div = vec2(screenSize.x * pixelSize / screenSize.y, pixelSize);
		uv = floor(uv * div)/div;
		
		//c64Colors
		vec3 c64col[16];
		c64col[0] = vec3(0.0,0.0,0.0);
		c64col[1] = vec3(62.0,49.0,162.0);
		c64col[2] = vec3(87.0,66.0,0.0);
		c64col[3] = vec3(140.0,62.0,52.0);
		c64col[4] = vec3(84.0,84.0,84.0);
		c64col[5] = vec3(141.0,71.0,179.0);
		c64col[6] = vec3(144.0,95.0,37.0);
		c64col[7] = vec3(124.0,112.0,218.0);
		c64col[8] = vec3(128.0,128.0,129.0);
		c64col[9] = vec3(104.0,169.0,65.0);
		c64col[10] = vec3(187.0,119.0,109.0);
		c64col[11] = vec3(122.0,191.0,199.0);
		c64col[12] = vec3(171.0,171.0,171.0);
		c64col[13] = vec3(208.0,220.0,113.0);
		c64col[14] = vec3(172.0,234.0,136.0);
		c64col[15] = vec3(255.0,255.0,255.0);

		vec3 samp = texture(sDiffMap, uv.xy).rgb;
		vec3 match = vec3(0.0,0.0,0.0);
		float best_dot = 8.0;

		for (int c=15;c>=0;--c) {
			float this_dot = distance(c64col[c]/255.0,samp);
			if (this_dot<best_dot) {
				best_dot=this_dot;
				match=c64col[c];
			}
		}

		vec3 color = vec3(match/255.0);
		vec2 xy = gl_FragCoord.xy * vec2(1, 1);
		int x = int(mod(xy.x, 4.0));
		int y = int(mod(xy.y, 4.0));
		
		vec3 rgb = texture(sDiffMap, uv.xy).rgb;
		vec3 finalRGB;
		finalRGB.r = find_closest(x, y, rgb.r);
		finalRGB.g = find_closest(x, y, rgb.g);
		finalRGB.b = find_closest(x, y, rgb.b);

		gl_FragColor = vec4(finalRGB*color, 1.0);
	}
#endif 
