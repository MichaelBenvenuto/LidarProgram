#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "pcap_read.h"
#include "shader.h"

int main(void) {

	if (!glfwInit()) {
		glfwTerminate();
		return 1;
	}

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

	//printf("%i %i\n", vert, frag);

	//system("pause");

	int size = 0;
	float* data = readPCAP(&size);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBO);

	glAttachShader(program, vert);
	glAttachShader(program, frag);

	glBindAttribLocation(program, 0, "vertex_position");
	glBindAttribLocation(program, 1, "vertex_color");

	glLinkProgram(program);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	free(data);

	glEnable(GL_DEPTH_TEST);
	float angle = 0;
	while (!glfwWindowShouldClose(window)) {

		glMatrixMode(GL_PROJECTION);
		glViewport(0, 0, 800, 800);
		//glFrustum(0, 800, 480, 0, 0, 2);
		//glOrtho(0, 800, 0, 480, 0, 2);
		glDepthRange(0, 10);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPointSize(1);

		glUseProgram(program);
		glBindVertexArray(VAO);

		glPushMatrix();
		glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
		glScalef(0.00001, 0.00001, 0.00001);
		glDrawArrays(GL_POINTS, 0, size / 1000);
		glPopMatrix();
		if (angle >= 360) {
			angle = 0;
		}
		angle += 0.002f;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}