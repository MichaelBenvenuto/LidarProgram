#include "h_lidar_math_common.h"

double interpolate(double alpha, double in_min, double in_max, double out_min, double out_max) {
	return ((alpha - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
}