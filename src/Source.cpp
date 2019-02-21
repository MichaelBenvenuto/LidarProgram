#include <GLFW/glfw3.h>
#include <math.h>

int main(void) {
	if (!glfwInit()) {
		glfwTerminate();
		return 1;
	}
	GLFWwindow* window = glfwCreateWindow(800, 480, "Lidar-Test", NULL, NULL);
	if (!window) {
		glfwTerminate();
		return 2;
	}

	glEnable(GL_DEPTH_TEST);
	glfwMakeContextCurrent(window);
	float angle = 0;
	while (!glfwWindowShouldClose(window)) {

		glViewport(0, 0, 800, 480);
		//glOrtho(0, 800, 0, 480, 0, 2);
		glDepthRange(0, 1);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		glPointSize(2);

		glPushMatrix();
		glRotatef(30.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(angle, 0.0f, 1.0f, 0.0f);
		glBegin(GL_POINTS);
		for (int x = -10; x <= 10; x++) { 
			for (int z = -10; z <= 10; z++) { 
				for (int y = -10; y <= 10; y++) { 
					glVertex3f(x * 0.07f, y * 0.07f, z * 0.07f);
				}
				
			}
		}
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
