#include "h_lidar_converter_common.h"

#include <string.h>

void load_packets(const uint8_t* data, int count, packet_t** out_data, int* data_count, GPSpacket_t** out_pos, int* pos_count) {

	packet_t* data_ptr = 0;
	GPSpacket_t* pos_ptr = 0;

	Gheader_t file_header;
	memcpy(&file_header, &data[0], sizeof(Gheader_t));
	
	int max_data = (count / sizeof(packet_t));
	int max_pos = (count / sizeof(GPSpacket_t));

	data_ptr = (packet_t*)calloc(max_data, sizeof(packet_t));
	pos_ptr = (GPSpacket_t*)calloc(max_pos, sizeof(GPSpacket_t));

	int data_c = 0, pos_c = 0;

	for (int i = sizeof(Gheader_t); i < count;) {
		Pheader_t packet_header;
		memcpy(&packet_header, &data[i], sizeof(Pheader_t));

		if (packet_header.orig_len == sizeof(packet_t)) {
			memcpy(&data_ptr[data_c], &data[i + sizeof(Pheader_t)], sizeof(packet_t));
			data_c++;
		}
		else if (packet_header.orig_len == sizeof(GPSpacket_t)) {

			GPSpacket_t temp_packet;
			memcpy(&temp_packet, &data[i + sizeof(Pheader_t)], sizeof(GPSpacket_t));

			int test = strcmp((char*)temp_packet.GPRMC_Sentence, (char*)pos_ptr[max(pos_c - 1, 0)].GPRMC_Sentence);
			if (test != 0) {

				float f1, f2;

				if (conv_NMEA(temp_packet, &f1, &f2)) {
					pos_ptr[pos_c] = temp_packet;
					pos_c++;
				}
			}
		}

		i += packet_header.orig_len + sizeof(Pheader_t);
	}

	*data_count = data_c;
	*pos_count = pos_c;

	printf("%i\n%i\n", data_c, pos_c);

	data_ptr = (packet_t*)realloc(data_ptr, (data_c) * sizeof(packet_t));
	pos_ptr = (GPSpacket_t*)realloc(pos_ptr, (pos_c) * sizeof(GPSpacket_t));

	*out_data = data_ptr;
	*out_pos = pos_ptr;

	printf("%p\n%p\n", data_ptr, pos_ptr);

}