#include "h_lidar_converter_common.h"

void packet_process(point_t* points, fdblock_t dblock, float next_angle, int i) {

	float a1, a2, a3;

	a1 = QUICKCAST_16T(dblock.data.azimuth) / 100.0f;
	a3 = next_angle;

	if (a3 < a1) {
		a3 = a3 + 360.0f;
	}

	a2 = (a1 + ((a3 - a1) / 2));

	a1 *= (3.14f) / 180.0f;
	a2 *= (3.14f) / 180.0f;
	a3 *= (3.14f) / 180.0f;

	for (int j = 0; j < 32; j++) {
		float distance = QUICKCAST_16T(dblock.data.channel_data[j].distance) / 100.0f;
		//printf("%f\n", distance);

		double channel = lookup_table[j % 16] * 3.14 / 180.0f;

		if (j < 16) {
			points[i * j] = atoxyz(channel, a1, distance);
		}
		else {
			points[i * j] = atoxyz(channel, a2, distance);
		}
	}
}