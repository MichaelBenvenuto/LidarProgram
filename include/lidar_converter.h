#ifndef LIDAR_DATA_CONVERTER
#define LIDAR_DATA_CONVERTER

#include "packet_data.h"
#include "point.h"
#include "color.h"

#include <stdio.h>
#include <stdint.h>

point_t *dblock(dblock_t data, uint16_t nextazimuth);
point_t *packet(packet_t data, int* out_size);


point_t *load_file_data(const uint8_t* data, int count, int *out_size);
point_t *load_file_data(const uint8_t* data, int count, int *out_size, int min, int max);

point_t *load_file_gps(const uint8_t* data, int count, int *out_size, int min, int max);

color_t *load_file_color(const uint8_t* data, int count, int *out_size);
color_t *load_file_color(const uint8_t* data, int count, int *out_size, int min, int max);

int conv_file(FILE* f, point_t* data, int count);

#endif
