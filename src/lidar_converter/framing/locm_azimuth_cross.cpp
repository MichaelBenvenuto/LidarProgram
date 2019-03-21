#include "h_lidar_converter_framing_common.h"

#include <stdint.h>

int azimuth_cross(packet_t packet, int* cross_at) {
	uint16_t last_azimuth = 0;
	for (int i = 0; i < 12; i++) {
		uint16_t azimuth = packet.data_blocks[i].data.azimuth;
		if (last_azimuth > azimuth) {
			*cross_at = i;
			return 1;
		}
	}
	return 0;
}