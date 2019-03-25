#include "h_lidar_math_curve_common.h"

int add_point(curve_t curve, double from, double to, int index) {
	curve.point_count++;

	curve.from = (double*)realloc(curve.from, curve.point_count * sizeof(double));
	curve.to = (double*)realloc(curve.to, curve.point_count * sizeof(double));

	if (!curve.from) return 0b010;
	if (!curve.to) return 0b100;

	for (int i = index; i < curve.point_count - 1; i++) {
		
		curve.from[i + 1] = curve.from[i];
		curve.to[i + 1] = curve.to[i];
	}
	curve.from[index] = from;
	curve.to[index] = to;

	return 0b000;
}