#ifndef LIDAR_MATHS
#define LIDAR_MATHS

#include "lidar_converter.h"

#define min(a,b) (a < b) ? a : b

double interpolate(double alpha, double in_min, double in_max, double out_min, double out_max);
point_t atoxyz(double a1, double a2, double distance);

#endif
