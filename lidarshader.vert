#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

out vec4 color;

void main() {
	color = vec4(vertex_color, 1.0);
	gl_Position = vec4(vertex_position / 5000.0, 1.0);
}