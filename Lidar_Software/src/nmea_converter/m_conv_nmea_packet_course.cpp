#include "h_nmea_converter_common.h"

int conv_NMEA(GPSpacket_t packet, double* latitude, double* longitude, double* t_course) {
	return conv_NMEA((char*)packet.GPRMC_Sentence, latitude, longitude, t_course);
}