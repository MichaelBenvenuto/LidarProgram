#include "h_lidar_converter_common.h"

color_t* load_file_color(const uint8_t* data, int count, int* out_size, int min, int max) {

	int packets_size;
	int color_size;

	packet_t* packets = load_packets_data(data, count, &packets_size);

	color_size = packets_size * 384;

	color_t* color = (color_t*)calloc(color_size, sizeof(color_t));

	int color_valid = 0;

	for (int i = 0; i < packets_size; i++) {
		for (int j = 0; j < 12; j++) {
			for (int k = 0; k < 32; k++) {
				uint8_t reflec = packets[i].data_blocks[j].data.channel_data[k].reflectivity;
				uint16_t distance = packets[i].data_blocks[j].data.channel_data[k].distance / 100.0;

				if (distance < max && distance > min) {
					color[color_valid].r = interpolatef(reflec, 0, 255, 0.0, 1.0);
					color[color_valid].g = interpolatef(reflec, 255, 0, 0.0, 1.0);
					color[color_valid].b = 0;

					color_valid++;

				}

			}
		}
	}

	*out_size = color_valid;

	color_t* c = color;

	color = (color_t*)realloc(color, color_valid * sizeof(color_t));

	if (c != color) {
		printf("MEMORY LEAK THERE!\n");
		free(c);
	}

	return color;
}