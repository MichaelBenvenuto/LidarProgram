#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

uniform mat4 camera_translation;
uniform mat4 projection;

out vec3 color;

void main() {
	color = vertex_color;
	gl_Position = projection * camera_translation * vec4(vertex_position / 1000, 1.0);
}