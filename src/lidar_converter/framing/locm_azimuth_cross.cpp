#include "h_lidar_converter_framing_common.h"

#include <stdint.h>

int azimuth_cross(packet_t packet_last, packet_t packet_current, int* cross_at) {
	*cross_at = -1;
	for (int i = 0; i < 12; i++) {
		uint16_t last_azimuth = 
			(i == 0) ? packet_last.data_blocks[11].data.azimuth : packet_current.data_blocks[i - 1].data.azimuth;
		uint16_t azimuth = packet_current.data_blocks[i].data.azimuth;
		if (last_azimuth > azimuth) {
			*cross_at = i;
			return 1;
		}
	}
	return 0;
}