#version 300 es // (GLSL ES 3.0)

// floatの制度: highp 高い, mediump 中くらい, lowp 低い
precision highp float;
out vec4 fragColor;
uniform vec2 u_resolution; // view port解像度
void main() {	
	vec2 pos = gl_FragCoord.xy / u_resolution.xy;
	fragColor = vec4(1.0, pos, 1.0);
}

