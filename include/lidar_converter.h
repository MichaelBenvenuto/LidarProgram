#ifndef LIDAR_DATA_CONVERTER
#define LIDAR_DATA_CONVERTER

#include "packet_data.h"

typedef struct point_t {
	float x, y, z;
};

point_t *dblock(dblock_t data, uint16_t nextazimuth);
point_t *packet(packet_t data, int* out_size);
point_t *file(const unsigned char* data, int count, int *out_size);

#endif
