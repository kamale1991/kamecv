#version 300 es // (GLSL ES 3.0)

// floatの制度: highp 高い, mediump 中くらい, lowp 低い
precision highp float;
out vec4 fragColor;
uniform vec2 u_resolution; // view port解像度

void main() {
	vec2 pos = gl_FragCoord.xy / u_resolution.xy;
	vec3 red = vec3(1.0, 0.0, 0.0);
	vec3 blue = vec3(0.0, 0.0, 1.0);
	vec3 col = mix(red, blue, pos.x);
	fragColor = vec4(col, 1.0);
}