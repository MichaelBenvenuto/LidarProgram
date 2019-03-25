#include "h_lidar_math_curve_common.h"

double calculate_curve(curve_t curve, double input) {

	if (input < curve.from[0]) {
		float a = curve.from[0];
		float b = curve.from[1];
		float a1 = curve.to[0];
		float b1 = curve.to[1];
		return interpolate(input, a, b, a1, b1);
	}
	else if (input > curve.from[curve.point_count - 1]) {
		float a = curve.from[curve.point_count - 2];
		float b = curve.from[curve.point_count - 1];
		float a1 = curve.to[curve.point_count - 2];
		float b1 = curve.to[curve.point_count - 1];
		return interpolate(input, a, b, a1, b1);
	}

	for (int i = 1; i < curve.point_count; i++) {
		float a = curve.from[i - 1];
		float b = curve.from[i];
		if (input > a && input < b) {
			float a1 = curve.to[i - 1];
			float b1 = curve.to[i];
			return interpolate(input, a, b, a1, b1);
		}
	}
}