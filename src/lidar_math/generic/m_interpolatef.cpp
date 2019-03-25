#include "h_lidar_math_generic_common.h"

float interpolatef(float alpha, float in_min, float in_max, float out_min, float out_max) {
	return ((alpha - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
}