#include "include/lidar_converter.h"

LidarConverter::LidarConverter(uint8_t *data, int count) {
	this->data = data;
	this->count = count;
}

LidarConverter::~LidarConverter() {
	free(data);
}

void LidarConverter::set_data(uint8_t* data, int count) {
	this->data = data;
	this->count = count;
}