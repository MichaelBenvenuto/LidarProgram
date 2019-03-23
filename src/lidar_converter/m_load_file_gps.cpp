#include "h_lidar_converter_common.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/matrix.hpp>
#include <glm/gtx/rotate_vector.hpp>

point_t *load_file_gps(const uint8_t* data, int count, int *out_size, int min, int max) {
	packet_t* packet_data = 0;
	GPSpacket_t* packet_pos = 0;

	int data_count, pos_count;

	load_packets(data, count, &packet_data, &data_count, &packet_pos, &pos_count);

	int point_size = data_count * 384;
	int point_valid = 0;

	point_t* points = (point_t*)calloc(point_size, sizeof(point_t));

	int interpolate_pos = 0;

	point_t* home = 0;

	for (int i = 0; i < data_count; i++) {
		packet_t packet = packet_data[i];
		for (int j = 0; j < 12; j++) {
			dblock_t data_block = packet.data_blocks[j].data;

			double a1, a2, a3;

			a1 = (data_block.azimuth / 100.0);
			a3 = (j == 11) ? 0 : (packet.data_blocks[j + 1].data.azimuth / 100.0);

			if (a1 < a3) {
				a3 += 360.0;
			}

			a2 = fmod((a1 + ((a3 - a1) / 2)), 360.0);

			a1 *= (3.14) / 180.0;
			a2 *= (3.14) / 180.0;
			a3 *= (3.14) / 180.0;

			for (int x = interpolate_pos; x < pos_count - 1; x++) {
				if (packet.timestamp >= packet_pos[x].timestamp && packet.timestamp <= packet_pos[x + 1].timestamp) {
					interpolate_pos = x;
					break;
				}
			}

			float lat1, lon1;
			float lat2, lon2;
			float lat3, lon3;

			float t_course;

			conv_NMEA(packet_pos[interpolate_pos], &lat1, &lon1, &t_course);
			conv_NMEA(packet_pos[interpolate_pos + 1], &lat3, &lon3);

			lat2 = interpolatef(packet.timestamp, packet_pos[interpolate_pos].timestamp, packet_pos[interpolate_pos + 1].timestamp, lat1, lat3);
			lon2 = interpolatef(packet.timestamp, packet_pos[interpolate_pos].timestamp, packet_pos[interpolate_pos + 1].timestamp, lon1, lon3);

			//printf("\n%f, %f\n%f, %f\n%f, %f\n", lat1, lon1, lat2, lon2, lat3, lon3);

			point_t p = atoxyz(lat1, lon1, 6371000.0);

			if (!home) {
				home = (point_t*)malloc(sizeof(point_t));
				if (!home) return 0;
				*home = p;
			}

			for (int k = 0; k < 32; k++) {

				double channel = lookup_table[k % 16] * (3.14 / 180.0);
				double distance = data_block.channel_data[k].distance / 500;

				//printf("%i\n", point_valid);

				if (distance > min && distance < max) {
					if (i < 16) {
						points[point_valid] = atoxyz(channel, a1, distance);
					}
					else {
						points[point_valid] = atoxyz(channel, a2, distance);
					}

					glm::vec3 pos(points[point_valid].x, points[point_valid].y, points[point_valid].z);

					pos = glm::rotate(pos, glm::radians(-t_course), glm::vec3(0, 0, 1));

					points[point_valid].x = pos.x + (p.x - home->x);
					points[point_valid].y = pos.y + (p.y - home->y);
					points[point_valid].z = pos.z;

					//printf("%f %f %f\n", points[point_valid].x, points[point_valid].y, points[point_valid].z);

					point_valid++;
				}
			}
		}
	}

	point_t* tpoint = points;

	points = (point_t*)realloc(points, point_valid * sizeof(point_t));

	if (tpoint != points) free(tpoint);

	*out_size = point_valid;

	free(home);
	free(packet_data);
	free(packet_pos);

	return points;

}