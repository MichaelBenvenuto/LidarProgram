#include "h_lidar_converter_framing_common.h"

frame_t* load_frames(const uint8_t* data, int count, int* out_size) {
	
	int data_size, pos_size;
	packet_t* packets_data;
	GPSpacket_t* packets_pos;

	load_packets(data, count, &packets_data, &data_size, &packets_pos, &pos_size);

	int frame_count = 0;

	int cross_at;
	printf("\n");
	for (int i = 1; i < data_size; i++) {
		if (azimuth_cross(packets_data[i - 1], packets_data[i], &cross_at)) {
			frame_count++;
		}
	}

	float lat, lon;

	conv_NMEA(packets_pos[0], &lat, &lon);

	/*frame_t* frames = (frame_t*)calloc(frame_count, sizeof(frame_t));

	if (!frames) return 0;*/

	printf("%u: ", packets_data[0].timestamp);
	printf("%f, %f: %u\n", lat, lon, packets_pos[0].timestamp);
	conv_NMEA(packets_pos[1], &lat, &lon);
	printf("%u: ", packets_data[1].timestamp);
	printf("%f, %f: %u\n", lat, lon, packets_pos[1].timestamp);

	return 0;
}