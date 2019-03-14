#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <GL/GLU.h>
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

	vert = loadShader("lidarshader.vert", GL_VERTEX_SHADER);
	frag = loadShader("lidarshader.frag", GL_FRAGMENT_SHADER);

	printf("%i, %i\n", vert, frag);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, &VBO[0]);

	glAttachShader(program, vert);
	glAttachShader(program, frag);

	glBindAttribLocation(program, 0, "vertex_position");
	glBindAttribLocation(program, 1, "vertex_color");

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked) {
		printf("\nerror\n");

		GLint len;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		
		char* log = (char*)calloc(len, sizeof(char));
		
		glGetProgramInfoLog(program, len, &len, log);
		printf("%s\n", log);

		free(log);

		system("pause");
		return 0;
	}

	printf("%i, %i\n", glGetAttribLocation(program, "vertex_position"), glGetAttribLocation(program, "vertex_color"));

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(point_t), data, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "vertex_position"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "vertex_position"));

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, size_c * sizeof(color_t), color, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "vertex_color"), 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(glGetAttribLocation(program, "vertex_color"));

	free(data);
	free(color);

	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window)) {

		glMatrixMode(GL_PROJECTION);
		glViewport(0, 0, 800, 800);
		glDepthRange(0, 10);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPointSize(1);

		glUseProgram(program);
		glBindVertexArray(VAO);

		glDrawArrays(GL_POINTS, 0, size);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}