#include "math_l.h"

#include <math.h>

double interpolate(double alpha, double in_min, double in_max, double out_min, double out_max) {
	return ((alpha - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
}

point_t atoxyz(double a1, double a2, double distance) {
	point_t p;

	p.x = distance * cos(a1) * sin(a2);
	p.y = distance * cos(a1) * cos(a2);
	p.z = distance * sin(a1);

	return p;
}