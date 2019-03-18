#include "h_lidar_math_common.h"

point_t atoxyz(double a1, double a2, double distance) {
	point_t p;

	p.x = (float)(distance * cos(a1) * sin(a2));
	p.y = (float)(distance * cos(a1) * cos(a2));
	p.z = (float)(distance * sin(a1));

	return p;
}