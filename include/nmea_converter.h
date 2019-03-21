#ifndef LIDAR_NMEA
#define LIDAR_NMEA

#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
	uint8_t header[42];
	uint8_t _PAD1_[198];
	uint32_t timestamp;
	uint32_t _PAD2_;
	int8_t GPRMC_Sentence[72];
	uint8_t _PAD3_[234];
} GPSpacket_t;
#pragma pack(pop)

//Converts GPRMC NMEA lat/lon data to a usable float
int conv_NMEA(const char* gprmc_message, float* latitude, float* longitude);
int conv_NMEA(GPSpacket_t packet, float* latitude, float* longitude);

int conv_NMEA(const char* gprmc_message, float* latitude, float* longitude, float* t_course);
int conv_NMEA(GPSpacket_t packet, float* latitude, float* longitude, float* t_course);

point_t conv_NMEA_geoloc(const char* message);
point_t* conv_NMEA_geoloc(GPSpacket_t* packet, int packet_count);

#endif
