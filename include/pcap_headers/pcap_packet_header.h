#ifndef LIDAR_PCAP_HEADER_PACKET
#define LIDAR_PCAP_HEADER_PACKET

#include <stdint.h>

#pragma pack(push, 1)

typedef struct {
	uint32_t ts_sec;         /* timestamp seconds */
	uint32_t ts_usec;        /* timestamp microseconds */
	uint32_t incl_len;       /* number of octets of packet saved in file */
	uint32_t orig_len;       /* actual length of packet */
} Pheader_t;

#pragma pack(push)

#endif