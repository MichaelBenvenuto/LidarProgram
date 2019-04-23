#ifndef LIDAR_SHADERS
#define LIDAR_SHADERS

const char* vertex_shader = R"(

#version 430

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;

uniform mat4 camera_translation;
uniform mat4 projection;

out vec3 color;

void main() {
	color = vertex_color;
	gl_Position = projection * camera_translation * vec4(vertex_position, 1.0);
	gl_PointSize = 2.0;
}

)";

const char* fragment_shader = R"(

#version 430

in vec3 color;
out vec4 frag_color;

void main() {
	frag_color = vec4(color,1.0);
}

)";

#endif