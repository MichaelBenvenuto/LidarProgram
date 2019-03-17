#ifndef LIDAR_SHADER
#define LIDAR_SHADER

int load_shader(const char* file, unsigned int shader_type);
int link_program(unsigned int program);

#endif