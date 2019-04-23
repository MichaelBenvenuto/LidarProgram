#ifndef LIDAR_CONVERTER_CLASS
#define LIDAR_CONVERTER_CLASS

#include "color.h"

#include <stdint.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define CAST(a,b) ((a*)b)

using namespace glm;

class LidarConverter {
public:
	LidarConverter(uint8_t* data, int count);
	~LidarConverter();

	void load_raw(vec3 **out_data, col3 **data, int *out_size);

	void load_raw1(vec3 **out_data, col3 **data, int *out_size);
	void load_raw2(vec3 **out_data, col3 **data, int *out_size);
	void load_raw3(vec3 **out_data, col3 **data, int *out_size);

	void set_data(uint8_t* data, int count);

private:
	int count;
	uint8_t* data;
};

#endif