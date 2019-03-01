#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

#include "lidar_converter.h"

#define QUICKCAST_16T(a) (*((uint16_t*)&a))
#define ROTATE_16T(a) ((a << 8) | (a >> 8))

const double lookup_table[16] = { -15,1,-13,-3,-11,5,-9,7,-7,9,-5,11,-3,13,-1,15 };

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

		a1 = ROTATE_16T(QUICKCAST_16T(dblock.data.azimuth));
		a3 = next_angle;

		if (a3 < a1) {
			a3 = a3 + 360.0f;
		}

		a2 = (a1 + ((a3 - a1) / 2)) / 100.0f;

		a1 *= (3.14) / 180.0f;
		a2 *= (3.14) / 180.0f;
		a3 *= (3.14) / 180.0f;

		for (int j = 0; j < 32; j++) {
			float distance = (ROTATE_16T(QUICKCAST_16T(dblock.data.channel_data[j].distance))) / 100.0f;

			double channel = lookup_table[j % 16] * 3.14 / 180.0f;

			if (j < 16) {
				points[i * j].x = distance * cos(channel)*sin(a1);
				points[i * j].y = distance * cos(channel)*cos(a1);
				points[i * j].z = distance * sin(channel);
			}
			else {
				points[i * j].x = distance * cos(channel)*sin(a2);
				points[i * j].y = distance * cos(channel)*cos(a2);
				points[i * j].z = distance * sin(channel);
			}
		}
}

point_t *packet(packet_t data, int *out_size) {
	*out_size = 384;
	point_t* points = (point_t*)calloc(384, sizeof(point_t));
	
	fdblock_t nblock;

	float angle;
	for (int i = 0; i < 11; i++) {
		angle = ROTATE_16T(QUICKCAST_16T(data.data_blocks[i + 1].data.azimuth));
		packet_process(points, data.data_blocks[i], angle, i);
	}
	angle = ROTATE_16T(QUICKCAST_16T(data.data_blocks[11].data.azimuth));
	packet_process(points, data.data_blocks[11], angle, 11);

	return points;
}

point_t *file(const unsigned char* data, int count, int* out_size) {
	int packets = 0;
	int packet_i = 0;

	for (int i = 0; i < count - 100; i++) {
		if (data[i] == 0xff) {
			int j;
			int ispacket = 1;
			for (j = i; j < i + 6; j++) {
				ispacket &= (data[j] == 0xff);
			}
			ispacket &= data[i + 42] == 0xff && data[i + 43] == 0xee;

			if (ispacket) {
				packets++;
			}
		}
	}

	*out_size = packets * 384;

	point_t* points = (point_t*)calloc(packets * 384, sizeof(point_t));
	packets = 0;

	for (int i = 0; i < count - 100; i++) {
		if (data[i] == 0xff) {
			int j;
			int ispacket = 1;
			for (j = i; j < i + 6; j++) {
				ispacket &= (data[j] == 0xff);
			}
			ispacket &= data[i + 42] == 0xff && data[i + 43] == 0xee;

			if (ispacket) {
				//printf("%X\n", i);

				packet_t packet_sto;

				memcpy(&packet_sto, &data[packet_i], sizeof(packet_t));

				int size = 0;
				point_t* store = packet(packet_sto, &size);

				memcpy(&points[packets * 384], store, 384);

				free(store);

				packets++;
				packet_i = i;

				i += sizeof(packet_t) - 1;
			}
		}
	}
	
	return points;
}