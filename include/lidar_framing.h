#ifndef LIDAR_FRAMING
#define LIDAR_FRAMING

#include "point.h"

#include <stdint.h>

#pragma pack(push, 1)

struct frame_t {
	int point_size;
	point_t* points;
};

#pragma pack(pop)

frame_t* load_frames(const uint8_t* data, int count, int* out_size);

#endif