#ifndef LIDAR_CONVERTER_COMMON_METHODS
#define LIDAR_CONVERTER_COMMON_METHODS

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "../../include/lidar_converter.h"
#include "../../include/nmea_converter.h"
#include "../../include/math_l.h"

#define QUICKCAST_16T(a) (*((uint16_t*)&a))
#define ROTATE_16T(a) (((a << 8) | (a >> 8)) & 0xffffu)

const double lookup_table[16] = { -15.0,1.0,-13.0,-3.0,-11.0,5.0,-9.0,7.0,-7.0,9.0,-5.0,11.0,-3.0,13.0,-1.0,15.0 };

void packet_process(point_t* points, fdblock_t dblock, float next_angle, int i);
void process_translation(double lat, double lon, point_t* points, point_t home, int start, int size, int total_size);

#endif