#version 300 es // (GLSL ES 3.0)

// floatの制度: highp 高い, mediump 中くらい, lowp 低い
precision highp float;
out vec4 fragColor;
uniform vec2 u_resolution; // view port解像度
uniform float u_time;

const float PI = 3.1415926;

float atan2(float y, float x) {
	if (x == 0.0) { 
		return sign(y) * PI * 0.5;
	} else {
		return atan(y, x);
	}
}

vec2 xy2pol(vec2 xy) {
	return vec2(atan2(xy.y, xy.x), length(xy));
}

vec2 pol2xy(vec2 pol) {
	return pol.y * vec2(cos(pol.x), sin(pol.x));
}

vec3 tex(vec2 st) { // s: 偏角, t: 動径
	vec3[3] col3 = vec3[](
		vec3(0.0, 0.0, 1.0),
		vec3(1.0,0.0,0.0),
		vec3(1.0)
	);
	st.s = st.s / PI + 1.0;
	int ind = int(st.s);
	vec3 col = mix(col3[ind % 2], col3[(ind + 1) % 2], fract(st.s));

	return mix(col3[2], col, st.t);
}

void main() {
	vec2 pos = gl_FragCoord.xy / u_resolution.xy;
	pos = 2.0 * pos.xy - vec2(1.0);
	pos = xy2pol(pos);
	fragColor = vec4(tex(pos), 1.0);
}