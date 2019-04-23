#include "h_lidar_converter_class_common.h"

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void LidarConverter::load_raw3(vec3 **out_data, col3 **out_color, int *out_size) {
	int data_limit = 0;

	for (int i = sizeof(Gheader_t); i < this->count;) {
		Pheader_t pack_header;
		memcpy(&pack_header, &this->data[i], sizeof(Pheader_t));
		if (pack_header.orig_len == sizeof(packet_t)) {
			data_limit++;
		}
		i += sizeof(Pheader_t) + pack_header.orig_len;
	}

	vec3* points = (vec3*)calloc(data_limit * 384, sizeof(vec3));
	col3* colors = (col3*)calloc(data_limit * 384, sizeof(col3));

	Gheader_t* glob_header = CAST(Gheader_t, this->data);

	int point_index = 0;

	uint16_t raw_azimuth;
	uint16_t last_raw_azimuth;
	uint16_t azimuth_diff_raw;
	uint16_t last_diff_raw;
	double azimuth;

	for (int i = sizeof(Gheader_t); i < this->count;) {
		Pheader_t pack_header;
		memcpy(&pack_header, &this->data[i], sizeof(Pheader_t));

		i += sizeof(Pheader_t);
		if (pack_header.orig_len == sizeof(packet_t)) {
			packet_t data_packet;
			memcpy(&data_packet, &this->data[i], sizeof(packet_t));

			int sequence = 0;
			for (int block = 0; block < 12; block++) {
				dblock_t data;
				memcpy(&data, &data_packet.data_blocks[block], sizeof(dblock_t));

				raw_azimuth = data.azimuth;
				if (block < 11) {
					dblock_t data_next;
					memcpy(&data_next, &data_packet.data_blocks[block + 1], sizeof(dblock_t));

					azimuth_diff_raw = (data_next.azimuth - raw_azimuth);
					last_diff_raw = azimuth_diff_raw;

					last_raw_azimuth = (raw_azimuth + (azimuth_diff_raw / 2)) % 36000;
				}
				else {
					raw_azimuth = last_raw_azimuth;
				}

				azimuth = last_raw_azimuth / 100.0;

				for (int pnt = 0; pnt < 32; pnt++) {
					if (pnt % 16 == 0) {
						sequence++;
					}


					vec3 p;
					double distance = data.channel_data[pnt].distance / 50.0;
					double angle_channel = (lookup_table[pnt % 16] / 100.0) * 3.14 / 180.0;
					double cos_iter = distance * cos(angle_channel);

					p.x = (float)(cos_iter * sin(azimuth));
					p.y = (float)(cos_iter * cos(azimuth));
					p.z = (float)(distance * sin(angle_channel));

					//printf("%f %f %f\n", p.x, p.y, p.z);

					col3 c;

					int intensity = ((data.channel_data[pnt % 16].reflectivity / 255.0) * 36000);

					c.r = cos(intensity - (3.14 / 2.0)); c.r *= c.r;
					c.g = sin(intensity); c.g *= c.g;
					c.b = cos(intensity); c.b *= c.b;

					points[point_index] = p;
					colors[point_index++] = c;

				}
			}
		}
		i += pack_header.orig_len;
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
}