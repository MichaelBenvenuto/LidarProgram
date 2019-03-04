#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "lidar_converter.h"
#include "nmea_converter.h"
#include "math_l.h"

#define QUICKCAST_16T(a) (*((uint16_t*)&a))
#define ROTATE_16T(a) (((a << 8) | (a >> 8)) & 0xffffu)

const double lookup_table[16] = { -15.0,1.0,-13.0,-3.0,-11.0,5.0,-9.0,7.0,-7.0,9.0,-5.0,11.0,-3.0,13.0,-1.0,15.0 };

point_t *dblock(dblock_t data, uint16_t nextazimuth) {
	float a1, a2, a3;
	a1 = (((uint16_t)data.azimuth[1] << 8) | data.azimuth[0]) / 100.0f;
	a3 = ROTATE_16T(nextazimuth) / 100.0f;
	
	if (a3 < a1) {
		a3 = a3 + 360;
	}
	
	a2 = (a1 + (a3 - a1) / 2.0) / 100.0f;

	

	a1 *= (3.14 / 180.0);
	a2 *= (3.14 / 180.0);
	a3 *= (3.14 / 180.0);

	point_t* points = (point_t*)calloc(32, sizeof(point_t));
	for (int i = 0; i < 32; i++) {
		cdata_t cdata = data.channel_data[i];
		float distance = ROTATE_16T(QUICKCAST_16T(cdata.distance)) / 100.0f;

		double w = lookup_table[i % 16];

		if (i < 16) {
			points[i].x = distance * (cos(w) * sin(a1));
			points[i].y = distance * (cos(w) * cos(a1));
			points[i].z = distance * (sin(w));
		}
		else {
			points[i].x = distance * (cos(w) * sin(a2));
			points[i].y = distance * (cos(w) * cos(a2));
			points[i].z = distance * (sin(w));
		}
	}

	return points;
}

void packet_process(point_t* points, fdblock_t dblock, float next_angle, int i) {

		float a1, a2, a3;

		a1 = QUICKCAST_16T(dblock.data.azimuth) / 100.0f;
		a3 = next_angle;

		if (a3 < a1) {
			a3 = a3 + 360.0f;
		}

		a2 = (a1 + ((a3 - a1) / 2));

		a1 *= (3.14f) / 180.0f;
		a2 *= (3.14f) / 180.0f;
		a3 *= (3.14f) / 180.0f;

		for (int j = 0; j < 32; j++) {
			float distance = QUICKCAST_16T(dblock.data.channel_data[j].distance) / 100.0f;
			//printf("%f\n", distance);

			double channel = lookup_table[j % 16] * 3.14 / 180.0f;

			if (j < 16) {
				points[i * j] = atoxyz(channel, a1, distance);
			}
			else {
				points[i * j] = atoxyz(channel, a2, distance);
			}
		}
}

void process_translation(double lat, double lon, point_t* points, point_t home, int start, int size, int total_size) {

	//printf("%i %i\n", start, size);
	//system("pause");
	point_t p = atoxyz(lat, lon, 63710000.0);
	
	int end = min(start + size, total_size);
	//printf("%f %f %f\n", home.x - p.x, home.y - p.y, home.z - p.z);
	
	for (int i = start; i < end; i++) {

		

		points[i].x += home.x - p.x;
		points[i].y += home.y - p.y;
		points[i].z += home.z - p.z;
	}
}

point_t *packet(packet_t data, int *out_size) {
	*out_size = 384;
	point_t* points = (point_t*)calloc(384, sizeof(point_t));
	
	fdblock_t nblock;

	float angle;
	for (int i = 0; i < 11; i++) {
		angle = QUICKCAST_16T(data.data_blocks[i + 1].data.azimuth) / 100.0f;
		packet_process(points, data.data_blocks[i], angle, i);
		//printf("%f\n", angle);
	}
	angle = QUICKCAST_16T(data.data_blocks[11].data.azimuth) / 100.0f;
	packet_process(points, data.data_blocks[11], angle, 11);

	return points;
}

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

int conv_NMEA(GPSpacket_t packet, float* latitude, float* longitude) {
	return conv_NMEA((char*)&packet.GPRMC_Sentence, latitude, longitude);
}