#include "h_lidar_converter_framing_common.h"

frame_t* load_frames(const uint8_t* data, int count, int* out_size) {
	
	int data_size, pos_size;
	packet_t* packets_data;
	GPSpacket_t* packets_pos;

	load_packets(data, count, &packets_data, &data_size, &packets_pos, &pos_size);

	int frame_count = 0;

	int cross_at;

	for (int i = 0; i < data_size; i++) {
		if (azimuth_cross(packets_data[i], &cross_at)) {
			frame_count++;
		}
	}

	printf("%i\n", frame_count);

	return 0;
}