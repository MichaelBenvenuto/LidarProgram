/*
#include "h_lidar_converter_class_common.h"

#include "include/nmea_converter.h"

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <glm/gtx/rotate_vector.hpp>

void LidarConverter::load_geo(vec3 **out_data, col3 **out_color, int *out_size) {

	int data_limit = (this->count / sizeof(packet_t)) * 384;

	vec3* points = (vec3*)calloc(data_limit, sizeof(vec3));
	col3* colors = (col3*)calloc(data_limit, sizeof(col3));

	Gheader_t* glob_header = CAST(Gheader_t, this->data);

	int point_index = 0;

	uint16_t raw_azimuth;
	uint16_t last_raw_azimuth;
	uint16_t azimuth_diff_raw;
	uint16_t last_diff_raw;
	double azimuth;

	int home_geo = 0;
	int home_translation = 0;
	double h_lat, h_lon;
	double heading;

	int change_count = 0;
	vec3 translation_last(0, 0, 0);
	vec3 translation(0,0,0);
	vec3 home(0, 0, 0);

	vec3 translation_final(0, 0, 0);

	double true_lat;
	double lat_sum = 0;
	int gps_count = 1;

	for (int i = sizeof(Gheader_t); i < this->count;) {
		Pheader_t* pack_header = CAST(Pheader_t, &this->data[i]);
		i += sizeof(Pheader_t);
		if (pack_header->orig_len == sizeof(packet_t)) {
			packet_t* data_packet = CAST(packet_t, &this->data[i]);

			int sequence = 0;
			for (int block = 0; block < 12; block++) {
				dblock_t* data = CAST(dblock_t, &data_packet->data_blocks[block].data);
				raw_azimuth = data->azimuth;
				if (block < 11) {
					dblock_t* data_next = CAST(dblock_t, &data_packet->data_blocks[block + 1].data);

					azimuth_diff_raw = (data_next->azimuth - raw_azimuth);
					last_diff_raw = azimuth_diff_raw;

					last_raw_azimuth = (raw_azimuth + (azimuth_diff_raw / 2)) % 36000;
				}
				else {
					raw_azimuth = last_raw_azimuth;
				}

				azimuth = raw_azimuth / 100.0;
				azimuth *= 3.14 / 180.0;

				for (int pnt = 0; pnt < 32; pnt++) {
					if (pnt % 16 == 0) {
						sequence++;
					}


					vec3 p;
					double distance = data->channel_data[pnt % 16].distance / 50.0;
					double angle_channel = lookup_table[pnt % 16] * (3.14 / 180);
					double cos_iter = distance * cos(angle_channel);

					p.x = (float)(cos_iter * sin(azimuth));
					p.y = (float)(cos_iter * cos(azimuth));
					p.z = (float)(distance * sin(angle_channel));

					p = rotateZ(p, (float)(heading));

					p += translation - home;

					col3 c;

					double intensity = ((data->channel_data[pnt % 16].reflectivity * 2 * 3.14) / 255.0);

					c.r = cos(intensity + (3.14 / 4.0)); c.r *= c.r;
					c.g = sin(intensity); c.g *= c.g;
					c.b = cos(intensity); c.b *= c.b;

					points[point_index] = p;
					colors[point_index++] = c;

				}
			}
		}
		else if (pack_header->orig_len == sizeof(GPSpacket_t)) {
			GPSpacket_t* packet = CAST(GPSpacket_t, &this->data[i]);
			double lat, lon;
			conv_NMEA(*packet, &lat, &lon, &heading);

			heading -= 180.0;

			lat *= 3.14 / 180.0;
			lon *= 3.14 / 180.0;
			heading *= 3.14 / 180.0;

			if (!home_geo) {
				h_lat = lat;
				h_lon = lon;

				home_geo = 1;
			}

			lat_sum += lat - h_lat;
			true_lat = lat_sum / (double)gps_count;

			translation.x = lon * cos(true_lat);
			translation.y = lat;

			if (!home_translation) {
				home = translation;
				home_translation = 1;
			}

			translation_final = (6731000.0f * (translation - home));
			translation_final.z = 0;

			if (translation != translation_last) {
				translation_last = translation;
				change_count++;
			}
			gps_count++;
		}
		
		i += pack_header->orig_len;
	}

	*out_data = (vec3*)realloc(points, sizeof(vec3) * point_index);
	*out_color = (col3*)realloc(colors, sizeof(col3) * point_index);

	if (points != *out_data) {
		free(points);
	}

	if (colors != *out_color) {
		free(colors);
	}

	*out_size = point_index;

}*/