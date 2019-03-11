#include "h_lidar_converter_common.h"

int conv_file(FILE* f, point_t *data, int count) {
	
	fprintf(f, "%c%c%c%c\n", (count >> 24) & 0xff, (count >> 16) & 0xff, (count >> 8) & 0xff, (count) & 0xff);
	
	return fwrite(data, sizeof(point_t), count, f);
}