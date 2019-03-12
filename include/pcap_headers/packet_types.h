#ifndef LIDAR_PACKET_TYPES
#define LIDAR_PACKET_TYPES

#include <stdint.h>

#include "pcap_global_header.h"
#include "pcap_packet_header.h"

#include "../packet_data.h"
#include "../nmea_converter.h"

#pragma pack(push, 1)

typedef struct {
	Pheader_t header;
	GPSpacket_t data;
} lpacket_pos_t;

typedef struct {
	Pheader_t header;
	packet_t data;
} lpacket_data_t;

#pragma pack(pop)

#endif