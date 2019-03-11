#ifndef LIDAR_DATA_CONVERTER
#define LIDAR_DATA_CONVERTER

#include "packet_data.h"
#include "point.h"

#include <stdio.h>

point_t *dblock(dblock_t data, uint16_t nextazimuth);
point_t *packet(packet_t data, int* out_size);
point_t *load_file(const unsigned char* data, int count, int *out_size);

int conv_file(FILE* f, point_t* data, int count);

#endif
