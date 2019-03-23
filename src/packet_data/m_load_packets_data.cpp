#include "h_packet_data_common.h"

packet_t* load_packets_data(const uint8_t* data, int count, int* out_size) {
	
	Gheader_t header;

	memcpy(&header, data, sizeof(Gheader_t));
	
	int packets = 0;

	for (int i = sizeof(Gheader_t); i < count;) {
		Pheader_t packet_header;
		memcpy(&packet_header, &data[i], sizeof(Pheader_t));

		if (packet_header.orig_len == sizeof(packet_t)) {
			packets++;
		}
		i += packet_header.orig_len + sizeof(Pheader_t);

	}

	*out_size = packets;
	
	packet_t* ptr_pack = (packet_t*)calloc(packets, sizeof(packet_t));

	if (!ptr_pack) {
		*out_size = -1;
		return 0;
	}

	packets = 0;
	for (int i = sizeof(Gheader_t); i < count;) {
		Pheader_t packet_header;
		memcpy(&packet_header, &data[i], sizeof(Pheader_t));

		if (packet_header.orig_len == sizeof(packet_t)) {

			memcpy(&ptr_pack[packets], &data[i + sizeof(Pheader_t)], sizeof(packet_t));

			packets++;
		}
		i += packet_header.orig_len + sizeof(Pheader_t);
	}

	return ptr_pack;
}