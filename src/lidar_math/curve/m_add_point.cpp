#include "h_lidar_math_curve_common.h"

int add_point(curve_t curve, double from, double to) {
	if (curve.point_count != curve.point_count) return -2;

	curve.point_count++;

	curve.from = (double*)realloc(curve.from, curve.point_count * sizeof(double));
	curve.to = (double*)realloc(curve.to, curve.point_count * sizeof(double));

	if (!curve.from) return 0;
	if (!curve.to) return -1;

	curve.from[curve.point_count - 1] = from;
	curve.to[curve.point_count - 1] = to;

	return 1;
}