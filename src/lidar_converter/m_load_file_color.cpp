#include "h_lidar_converter_common.h"

color_t* load_file_color(const uint8_t* data, int count, int* out_size) {

	int packets_size;
	int color_size;

	packet_t* packets = load_packets_data(data, count, &packets_size);

	color_size = packets_size * 384;

	color_t* color = (color_t*)calloc(color_size, sizeof(point_t));

	if (!color) {
		return 0;
	}

	for(int i = 0; i < packets_size; i++){
		for (int j = 0; j < 12; j++) {
			for (int k = 0; k < 32; k++) {
				uint8_t reflec = packets[i].data_blocks[j].data.channel_data[k].reflectivity;

				int color_i = (i * 384) + (j * 32) + k;

				color[color_i].r = interpolate(reflec, 0, 255, 0.0, 1.0);
				color[color_i].g = interpolate(reflec, 255, 0, 0.0, 1.0);
				color[color_i].b = 0;

			}
		}
	}

	return color;
}