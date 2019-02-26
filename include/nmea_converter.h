#ifndef LIDAR_NMEA
#define LIDAR_NMEA

//Converts GPRMC NMEA lat/lon data to a usable float
int conv_NMEA(const char* gprmc_message, float* latitude, float* longitude);

#endif
