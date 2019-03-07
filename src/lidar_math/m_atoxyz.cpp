#include "h_lidar_math_common.h"

point_t atoxyz(double a1, double a2, double distance) {
	point_t p;

	p.x = distance * cos(a1) * sin(a2);
	p.y = distance * cos(a1) * cos(a2);
	p.z = distance * sin(a1);

	return p;
}