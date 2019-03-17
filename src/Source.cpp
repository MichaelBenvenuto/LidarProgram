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

#include "../include/packet_data.h"
#include "../include/lidar_converter.h"
#include "../include/nmea_converter.h"
#include "../include/shader.h"

int main(void) {

	FILE* f = fopen("C:\\Users\\Michael\\Desktop\\VELODYNE\\VLP-16 Sample Data\\2015-07-23-14-37-22_Velodyne-VLP-16-Data_Downtown 10Hz Single.pcap", "rb");

	fseek(f, 0, SEEK_END);
	int sizes = ftell(f);
	rewind(f);

	unsigned char* pcap_buffer = (unsigned char*)calloc(sizes + 1, sizeof(unsigned char));
	size_t size2 = fread_s(pcap_buffer, sizes + 1, sizeof(unsigned char), sizes, f);

	int size = 0;
	int size_c = 0;

	point_t* data = load_file_data(pcap_buffer, sizes, &size, 20, 10000);
	color_t* color = load_file_color(pcap_buffer, sizes, &size_c, 20, 10000);

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

	vert = load_shader("shaders/lidarshader.vert", GL_VERTEX_SHADER);
	frag = load_shader("shaders/lidarshader.frag", GL_FRAGMENT_SHADER);

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
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(point_t), data, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "vertex_position"));

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, size_c * sizeof(color_t), color, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "vertex_color"), 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(glGetAttribLocation(program, "vertex_color"));

	GLint location_camera = glGetUniformLocation(program, "camera_translation");
	GLint location_projection = glGetUniformLocation(program, "projection");

	free(data);
	free(color);

	glm::vec3 eye;

	double angle = 0;
	double radius = 2;

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {

		eye = glm::vec3(radius * cos(angle) * cos(0.5), radius * sin(angle) * cos(0.5), sin(0.5));

		glm::vec3 lookat(0, 0, 0);
		glm::vec3 direction = glm::normalize(eye - lookat);

		glm::vec3 up(0, 0, 1);
		glm::vec3 right = glm::normalize(glm::cross(up, direction));
		glm::vec3 camup = glm::normalize(glm::cross(direction, right));

		glm::mat4 camera = glm::lookAt(eye, glm::vec3(0, 0, 0), up);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glViewport(0, 0, 800, 800);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPointSize(1);

		glUseProgram(program);
		glBindVertexArray(VAO);

		glUniformMatrix4fv(location_camera, 1, GL_FALSE, glm::value_ptr(camera));
		glUniformMatrix4fv(location_projection, 1, GL_FALSE, glm::value_ptr(projection));

		glDrawArrays(GL_POINTS, 0, size);

		angle = fmod(angle + 0.01, 360);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}