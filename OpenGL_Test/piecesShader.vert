#version 430 core

layout(location = 0) in ivec2 a_position;
layout(location = 1) in int a_tex;

out int vertTex;

void main(){
	vertTex = a_tex;
	vec2 position = vec2(-1.0 + float(a_position.x)/4 + 0.125, -1.0 + float(a_position.y)/4 + 0.125);
	gl_Position = vec4(position, 0.0, 1.0);
}