#include "h_nmea_converter_common.h"

#include "../../include/math/math_generic.h"

point_t conv_NMEA_geoloc(const char* message) {
	float lat, lon;

	conv_NMEA(message, &lat, &lon);

	return atoxyz(lat, lon, 6371);
}