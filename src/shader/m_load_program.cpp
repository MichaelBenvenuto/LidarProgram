#define _CRT_SECURE_NO_WARNINGS

#include "h_shader_common.h"

int load_shader(const char* file, unsigned int shader_type) {
	FILE* f = fopen(file, "r");

	if (!f) {
		printf("Failed to open file\n");
		return 0;
	}

	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	rewind(f);

	char* shader_c = (char*)calloc(size, sizeof(char));

	if (!shader_c) return -1;

	int read = fread(shader_c, sizeof(char), size, f);

	GLuint shader_id = glCreateShader(shader_type);

	glShaderSource(shader_id, 1, &shader_c, &size);
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

	fclose(f);

	return shader_id;
}