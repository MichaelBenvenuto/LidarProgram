#include "h_shader_common.h"

int link_program(unsigned int program) {
	glLinkProgram(program);

	int linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked) {
		printf("\nLinker Error: Program %i\n", program);
		int len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		char* log = (char*)calloc(len, sizeof(char));

		glGetProgramInfoLog(program, len, &len, log);
		printf("%s\n", log);

		free(log);
		return 0;
	}

	return 1;
}