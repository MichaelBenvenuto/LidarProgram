#include "../include/shaders/shaders.h"

#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <GL/GLU.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "include/color.h"
#include "include/lidar_converter.h"

#include "../include/shader.h"

using namespace glm;

int main(void) {

	FILE* f = fopen("C:\\Users\\Michael\\Desktop\\VELODYNE\\VLP-16 Sample Data\\2015-07-23-14-37-22_Velodyne-VLP-16-Data_Downtown 10Hz Single.pcap", "rb");

	fseek(f, 0, SEEK_END);
	int sizes = ftell(f);
	rewind(f);

	unsigned char* pcap_buffer = (unsigned char*)calloc((size_t)(sizes)+1ull, sizeof(unsigned char));

	if (!pcap_buffer) {
		return -2;
	}

	size_t size2 = fread_s(pcap_buffer, (size_t)(sizes)+1ull, sizeof(unsigned char), sizes, f);

	int size = 0;

	LidarConverter* lconv = new LidarConverter((uint8_t*)pcap_buffer, sizes);

	vec3* data;
	col3* color;

	lconv->load_raw(&data, &color, &size);

	if (!glfwInit()) {
		glfwTerminate();
		return 1;
	}

	free(pcap_buffer);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 800, "Lidar-Test", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return 2;
	}
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();

	if (err != GLEW_OK) {
		printf("%i\n", err);
		system("pause");
		return 3;
	}

	GLuint VAO;
	GLuint VBO[2];

	GLuint vert, frag;
	GLuint program = glCreateProgram();

	vert = load_shader(vertex_shader, GL_VERTEX_SHADER);
	frag = load_shader(fragment_shader, GL_FRAGMENT_SHADER);

	if (!vert || !frag) {
		system("pause");
		return -1;
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, &VBO[0]);

	glAttachShader(program, vert);
	glAttachShader(program, frag);

	glBindAttribLocation(program, 0, "vertex_position");
	glBindAttribLocation(program, 1, "vertex_color");
	//glBindAttribLocation(program, 2, "camera_translation");

	if (!link_program(program)) {
		system("pause");
		return -1;
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(vec3), data, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "vertex_position"));

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(col3), color, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "vertex_color"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(glGetAttribLocation(program, "vertex_color"));

	GLint location_camera = glGetUniformLocation(program, "camera_translation");
	GLint location_projection = glGetUniformLocation(program, "projection");

	free(data);
	free(color);

	vec3 eye;

	double angle = 0;
	double radius = 500;

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_PROGRAM_POINT_SIZE);
	while (!glfwWindowShouldClose(window)) {

		eye = vec3((radius * cos(angle) * cos(0.5)), (radius * sin(angle) * cos(0.5)), radius * sin(0.5));

		vec3 lookat(0, 0, 0);
		vec3 direction = normalize(eye - lookat);

		vec3 up(0, 0, 1);
		vec3 right = normalize(cross(up, direction));
		vec3 camup = normalize(cross(direction, right));

		mat4 camera = lookAt(eye, vec3(0, 0, 0), up);
		mat4 projection = perspective(radians(45.0f), 1.0f, 0.1f, 1000.0f);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, 800, 800);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPointSize(1);

		glUseProgram(program);
		glBindVertexArray(VAO);

		glUniformMatrix4fv(location_camera, 1, GL_FALSE, value_ptr(camera));
		glUniformMatrix4fv(location_projection, 1, GL_FALSE, value_ptr(projection));

		glDrawArrays(GL_POINTS, 0, 439887);

		angle = fmod(angle + 0.0001, 360);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}