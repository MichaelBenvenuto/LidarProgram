#include "h_lidar_converter_common.h"

void process_translation(double lat, double lon, point_t* points, point_t home, int start, int size, int total_size) {

	//printf("%i %i\n", start, size);
	//system("pause");
	point_t p = atoxyz(lat, lon, 63710000.0);

	int end = min(start + size, total_size - 1);
	//printf("%f %f %f\n", home.x - p.x, home.y - p.y, home.z - p.z);

	for (int i = start; i < end; i++) {



		points[i].x += p.x - home.x;
		points[i].y += p.y - home.y;
		points[i].z += p.z - home.z;
	}
}