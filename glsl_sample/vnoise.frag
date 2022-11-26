#version 300 es // (GLSL ES 3.0)

// floatの制度: highp 高い, mediump 中くらい, lowp 低い

precision highp float;
out vec4 fragColor;
uniform vec2 u_resolution; // view port解像度
uniform float u_time;

const float PI = 3.1415926;

uvec3 k3 = uvec3(0x456789abu, 0x6789ab45u, 0x89ab4567u);
uvec3 u3 = uvec3(1, 2, 3);
const uint UINT_MAX = 0xffffffffu;

uvec2 uhash22(uvec2 n) {
	n ^= (n.yx << u3.xy);
	n ^= (n.yx >> u3.xy);
	n *= k3.xy;
	n ^= (n.yx << u3.xy);
	return n * k3.xy;
}


float hash21(vec2 p) {
	uvec2 n = floatBitsToUint(p);
	return float(uhash22(n).x) / float(UINT_MAX);
}

float vnoise21(vec2 p) {
	vec2 n = floor(p);
	float[4] v;
	for (int j = 0; j < 2; j++) {
		for (int i = 0; i < 2; i++) { 
			v[i + 2 * j] = hash21( n + vec2(i,j) );
		}
	}
}