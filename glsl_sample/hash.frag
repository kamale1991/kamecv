#version 300 es // (GLSL ES 3.0)

// floatの制度: highp 高い, mediump 中くらい, lowp 低い

precision highp float;
out vec4 fragColor;
uniform vec2 u_resolution; // view port解像度
uniform float u_time;

const float PI = 3.1415926;

uint k = 0x456789abu;
uvec3 k3 = uvec3(0x456789abu, 0x6789ab45u, 0x89ab4567u);
uvec3 u3 = uvec3(1, 2, 3);
const uint UINT_MAX = 0xffffffffu;

uint uhash11(uint n) {
	n ^= (n << 1);
	n ^= (n >> 1);
	n *= k;
	n ^= (n << 1);
	return n * k;
}

float hash11(float p) {
	uint n = floatBitsToUint(p);
	return float(uhash11(n)) / float(UINT_MAX);
}

uvec2 uhash22(uvec2 n) {
	n ^= (n.yx << u3.xy);
	n ^= (n.yx >> u3.xy);
	n *= k3.xy;
	n ^= (n.yx << u3.xy);
	return n * k3.xy;
}

uvec3 uhash33(uvec3 n) {
	n ^= (n.yzx << u3);
	n ^= (n.yzx >> u3);
	n *= k3;
	n ^= (n.yzx << u3);
	return n * k3;
}

vec2 has22(vec2 p) {
	uvec2 n = floatBitsToUint(p);
	return vec2(uhash22(n)) / vec2(UINT_MAX);
}

vec3 hash33(vec3 p) {
	uvec3 n = floatBitsToUint(p);
	return vec3(uhash33(n)) / vec3(UINT_MAX);
}

float hash21(vec2 p) {
	uvec2 n = floatBitsToUint(p);
	return float(uhash22(n).x) / float(UINT_MAX);
}

float hash31(vec3 p) {
	uvec3 n = floatBitsToUint(p);
	return float(uhash33(n).x) / float(UINT_MAX);
}

void main() {
	float time = floor(60.0 * u_time);
	vec2 pos = gl_FragCoord.xy + time;
	// fragColor.rgb = vec3(hash11(pos.x));
	fragColor.rgb = vec3(hash21(pos.xy));
	// fragColor.rgb = vec3(hash31(pos.xyx));
	fragColor.a = 1.0;
}