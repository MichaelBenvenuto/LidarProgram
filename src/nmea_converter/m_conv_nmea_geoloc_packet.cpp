#include "h_nmea_converter_common.h"

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

point_t* conv_NMEA_geoloc(GPSpacket_t* packet, int packet_count) {
	
	point_t* points = (point_t*)calloc(packet_count, sizeof(point_t));
	
	point_t* home = 0;

	for (int i = 0; i < packet_count; i++) {
		points[i] = conv_NMEA_geoloc((char*)packet[i].GPRMC_Sentence);
		if (!home) {
			home = &points[i];
		}

		points[i].x -= home->x;
		points[i].y -= home->y;
		points[i].z = 0;

		//printf("%s%f %f %f\n\n", packet[i].GPRMC_Sentence, points[i].x, points[i].y, points[i].z);

		//system("pause");

	}

	return points;
}