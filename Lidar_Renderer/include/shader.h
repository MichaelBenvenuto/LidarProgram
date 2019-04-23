#ifndef LIDAR_SHADER_LOADER
#define LIDAR_SHADER_LOADER

int load_shader(const char* text, unsigned int shader_type);
int link_program(unsigned int program);

#endif