#define _CRT_SECURE_NO_WARNINGS

#include "h_shader_common.h"

int load_shader(const char* file, unsigned int shader_type) {

	int size = strlen(file);

	GLuint shader_id = glCreateShader(shader_type);

	glShaderSource(shader_id, 1, &file, &size);
	glCompileShader(shader_id);

	GLint status;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE) {
		printf("Compile failed! %i\n", shader_type);

		int log_size;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &log_size);

		char* log_c = (char*)calloc(log_size, sizeof(char));

		glGetShaderInfoLog(shader_id, log_size, &log_size, log_c);

		printf("%s\n", log_c);

		free(log_c);

		return 0;
	}

	return shader_id;
}