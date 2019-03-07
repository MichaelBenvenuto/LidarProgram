#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <gl/GL.h>
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

	//Seek to the end of the file
	fseek(f, 0, SEEK_END);
	int sizes = ftell(f); //Retreive and store the size of the file
	rewind(f); //Undo that seek

			   //Prepare the array for storing the file data
	unsigned char* pcap_buffer = (unsigned char*)calloc(sizes + 1, sizeof(unsigned char));
	size_t size2 = fread_s(pcap_buffer, sizes + 1, sizeof(unsigned char), sizes, f);

	int size = 0;

	point_t* data = file(pcap_buffer, sizes, &size);

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

	//printf("%i %i\n", vert, frag);

	//system("pause");

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

	printf("%i\n", size);

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
		glColor3f(1, 1, 1);
		glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
		glScalef(0.001f, 0.001f, 0.001f);

		glDrawArrays(GL_POINTS, 0, size);

		glScalef(100000, 100000,100000);

		glBegin(GL_LINES);

		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);

		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 1, 0);

		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);

		glEnd();

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