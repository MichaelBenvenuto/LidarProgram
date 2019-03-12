#ifndef LIDAR_PCAP_HEADER_GLOBAL
#define LIDAR_PCAP_HEADER_GLOBAL

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
	uint32_t magic_number;   /* magic number */
	uint16_t version_major;  /* major version number */
	uint16_t version_minor;  /* minor version number */
	int32_t  thiszone;       /* GMT to local correction */
	uint32_t sigfigs;        /* accuracy of timestamps */
	uint32_t snaplen;        /* max length of captured packets, in octets */
	uint32_t network;		 /* data link type */
} Gheader_t;

#pragma pack(pop)

#endif