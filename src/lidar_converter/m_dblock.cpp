#include "h_lidar_converter_common.h"

point_t *dblock(dblock_t data, uint16_t nextazimuth) {
	double a1, a2, a3;
	a1 = (((uint16_t)data.azimuth[1] << 8) | data.azimuth[0]) / 100.0f;
	a3 = ROTATE_16T(nextazimuth) / 100.0f;

	if (a3 < a1) {
		a3 = a3 + 360;
	}

	a2 = (a1 + (a3 - a1) / 2.0) / 100.0f;



	a1 *= (3.14 / 180.0);
	a2 *= (3.14 / 180.0);
	a3 *= (3.14 / 180.0);

	point_t* points = (point_t*)calloc(32, sizeof(point_t));
	for (int i = 0; i < 32; i++) {
		cdata_t cdata = data.channel_data[i];
		float distance = ROTATE_16T(QUICKCAST_16T(cdata.distance)) / 100.0f;

		double w = lookup_table[i % 16];

		if (i < 16) {
			points[i].x = (float)(distance * (cos(w) * sin(a1)));
			points[i].y = (float)(distance * (cos(w) * cos(a1)));
			points[i].z = (float)(distance * (sin(w)));
		}
		else {
			points[i].x = (float)(distance * (cos(w) * sin(a2)));
			points[i].y = (float)(distance * (cos(w) * cos(a2)));
			points[i].z = (float)(distance * (sin(w)));
		}
	}

	return points;
}