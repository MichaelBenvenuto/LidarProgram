#include <memory.h>

#include "h_lidar_converter_common.h"

point_t *load_file_data(const uint8_t* data, int count, int* out_size) {
	int packets_size;
	int point_size;

	packet_t* packets = load_packets_data(data, count, &packets_size);
	point_size = packets_size * 384;

	*out_size = point_size;

	point_t* points = (point_t*)calloc(point_size, sizeof(point_t));

	if (!points) return 0;

	for (int i = 0; i < packets_size; i++) {
		packet_t packet = packets[i];
		for (int j = 0; j < 12; j++) {
			dblock_t data_block = packet.data_blocks[j].data;

			double a1, a2, a3;

			a1 = (data_block.azimuth / 100.0);
			a3 = (j == 11) ? 0 : (packet.data_blocks[j + 1].data.azimuth / 100.0);

			if (a1 < a3) {
				a3 += 360.0;
			}

			a2 = fmod((a1 + ((a3 - a1) / 2)), 360.0);

			a1 *= (3.14) / 180.0;
			a2 *= (3.14) / 180.0;
			a3 *= (3.14) / 180.0;


			for (int k = 0; k < 32; k++) {

				double channel = lookup_table[k % 16] * (3.14 / 180.0);

				if (i < 16) {
					points[(i * 384) + (j * 31) + k] = atoxyz(channel, a1, data_block.channel_data[k].distance / 100.0);
				}
				else {
					points[(i * 384) + (j * 31) + k] = atoxyz(channel, a2, data_block.channel_data[k].distance / 100.0);
				}
			}
		}
	}

	free(packets);

	return points;
}