#include "h_lidar_converter_common.h"

point_t *packet(packet_t data, int *out_size) {
	*out_size = 384;
	point_t* points = (point_t*)calloc(384, sizeof(point_t));

	fdblock_t nblock;

	float angle;
	for (int i = 0; i < 11; i++) {
		angle = QUICKCAST_16T(data.data_blocks[i + 1].data.azimuth) / 100.0f;
		packet_process(points, data.data_blocks[i], angle, i);
		//printf("%f\n", angle);
	}
	angle = QUICKCAST_16T(data.data_blocks[11].data.azimuth) / 100.0f;
	packet_process(points, data.data_blocks[11], angle, 11);

	return points;
}