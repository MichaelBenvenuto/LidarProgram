#include "h_lidar_math_curve_common.h"

int remove_point(curve_t curve) {
	curve.point_count--;

	curve.from = (double*)realloc(curve.from, curve.point_count * sizeof(double));
	curve.to = (double*)realloc(curve.to, curve.point_count * sizeof(double));

	if (!curve.from) return 0b010;
	if (!curve.to) return 0b100;

	return 0b000;
}