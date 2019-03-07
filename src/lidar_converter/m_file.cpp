#include <memory.h>

#include "h_lidar_converter_common.h"

point_t *file(const unsigned char* data, int count, int* out_size) {
	int packets = 0;
	int pos_packets = 0;
	int packet_i = 0;

	for (int i = 0; i < count - 100; i++) {
		if (data[i] == 0xff) {
			int j;
			uint8_t ispacket = 0b11;
			for (j = i; j < i + 6; j++) {
				ispacket = ispacket & (data[j] == 0xff) | (data[j] == 0xff) << 1;
			}
			ispacket = ispacket & ((data[i + 42] == 0xff && data[i + 43] == 0xee) | (data[i + offsetof(GPSpacket_t, GPRMC_Sentence)] == 0x24) << 1);

			//printf("%X ", ispacket);

			if (ispacket & 0b01) {
				packets++;
				//if(packets >= 1000) break;
			}
			if (ispacket & 0b10) {

			}
		}

	}

	*out_size = packets * 384;

	point_t* points = (point_t*)calloc(packets * 384, sizeof(point_t));
	packets = 0;


	int pos_i = 0, last_pos = 0;
	int pos = 0;
	int total_packets = 0;

	int last_data = 0;

	point_t* home = 0;

	int pos_start = 0;

	for (int i = 0; i < count - 100; i++) {
		if (data[i] == 0xff) {
			int j;
			uint8_t ispacket = 0b11;
			for (j = i; j < i + 6; j++) {
				ispacket = ispacket & (data[j] == 0xff) | (data[j] == 0xff) << 1;
			}
			ispacket = ispacket & ((data[i + 42] == 0xff && data[i + 43] == 0xee) | (data[i + offsetof(GPSpacket_t, GPRMC_Sentence)] == 0x24) << 1);

			//printf("%X ", ispacket);

			if (ispacket & 0b1) {
				//printf("%X\n", i);

				packet_t packet_sto;

				memcpy(&packet_sto, &data[i], sizeof(packet_t));

				int size = 0;
				point_t* store = packet(packet_sto, &size);

				memcpy(&points[packets * 384], store, 384);

				free(store);

				last_data = total_packets;

				packets++;
				packet_i = i;


				i += sizeof(packet_t) - 1;
				//if(packets >= 1000) break;
			}
			else if (ispacket & 0b10) {
				GPSpacket_t pos_sto;

				memcpy(&pos_sto, &data[i], sizeof(GPSpacket_t));

				float lat, lon;
				conv_NMEA(pos_sto, &lat, &lon);

				lat *= (3.14 / 180.0);
				lon *= (3.14 / 180.0);

				point_t xyz = atoxyz(lat, lon, 6371000.0);

				if (!home) {
					home = (point_t*)malloc(sizeof(point_t));
					home->x = xyz.x;
					home->y = xyz.y;
					home->z = xyz.z;

				}

				process_translation(lat, lon, points, *home, pos_start * 384, (total_packets - last_pos) * 384, *out_size);
				pos_start += (total_packets - last_pos);

				last_pos = total_packets;

				pos++;
				pos_i = i;

				i += sizeof(GPSpacket_t) - 1;
			}

			if (ispacket) {
				total_packets++;
			}

		}
	}

	//free(home);

	return points;
}