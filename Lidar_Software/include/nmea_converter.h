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

//Converts GPRMC NMEA lat/lon data to a usable double
int conv_NMEA(const char* gprmc_message, double* latitude, double* longitude);
int conv_NMEA(GPSpacket_t packet, double* latitude, double* longitude);

int conv_NMEA(const char* gprmc_message, double* latitude, double* longitude, double* t_course);
int conv_NMEA(GPSpacket_t packet, double* latitude, double* longitude, double* t_course);

GPSpacket_t* load_packets_pos(const uint8_t* data, int count, int* out_size);

#endif
