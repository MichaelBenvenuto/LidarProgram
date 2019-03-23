#include "h_lidar_converter_common.h"

#include <memory.h>

point_t *load_file_data(const uint8_t* data, int count, int *out_size, int min, int max) {
	int packets_size;
	int point_size;

	packet_t* packets = load_packets_data(data, count, &packets_size);
	point_size = packets_size * 384;

	point_t* points = (point_t*)calloc(point_size, sizeof(point_t));

	if (!points) return 0;

	int point_valid = 0;

	int should_break = 0;

	for (int i = 0; i < packets_size; i++) {
		packet_t packet = packets[i];
		for (int j = 0; j < 12; j++) {
			dblock_t data_block = packet.data_blocks[j].data;

			double a1, a2 = 0, a3;

			a1 = (data_block.azimuth / 100.0);
			a3 = (j == 11) ? (packets[i + 1].data_blocks[0].data.azimuth / 100.0) : (packet.data_blocks[j + 1].data.azimuth / 100.0);

			if (a1 < a3) {
				a3 += 360.0;
				should_break = 1;
			}

			a2 = (a1 + ((a3 - a1) / 2));

			if (a2 > 360.0) {
				a2 -= 360.0;
			}

			printf("%f %f %f\n", a1, a2, a3);

			a1 *= (3.14) / 180.0;
			a2 *= (3.14) / 180.0;
			a3 *= (3.14) / 180.0;


			for (int k = 0; k < 32; k++) {

				double channel = lookup_table[k % 16] * (3.14 / 180.0);
				double distance = data_block.channel_data[k].distance / 100.0;

				//printf("%i\n", point_valid);

				if (distance > min && distance < max) {
					if (i < 16) {
						points[point_valid] = atoxyz(channel, a1, distance);
					}
					else {
						points[point_valid] = atoxyz(channel, a2, distance);
					}
					point_valid++;
				}
			}
		}
		if (should_break) break;
	}

	*out_size = point_valid;

	point_t* p = points;

	points = (point_t*)realloc(points, point_valid * sizeof(point_t));

	if (p != points) {
		printf("MEMORY LEAK HERE!\n");
		free(p);
	}

	free(packets);

	return points;
}