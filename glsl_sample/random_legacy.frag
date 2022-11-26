#version 300 es // (GLSL ES 3.0)

// floatの制度: highp 高い, mediump 中くらい, lowp 低い

precision highp float;
out vec4 fragColor;
uniform vec2 u_resolution; // view port解像度
uniform float u_time;

const float PI = 3.1415926;

int channel;

float fracSin11(float x) {
	return fract(1000.0 * sin(x));
}

float fractSin21(vec2 xy) {
	return fract(sin(dot(xy, vec2(12.9898, 78.233))) * 43758.5453123);
}

void main() {
	vec2 pos = gl_FragCoord.xy;
	pos += floor(60.0 * u_time);
	channel = int(2.0 * gl_FragCoord.x / u_resolution.x);
	if (channel == 0) {
		fragColor = vec4(fracSin11(pos.x));
	} else {
		fragColor = vec4(fractSin21(pos.xy / u_resolution.xy));
	}
	fragColor.a = 1.0;
}