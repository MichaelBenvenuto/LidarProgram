#ifndef LIDAR_PDATA
#define LIDAR_PDATA

#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
	uint8_t distance[2];
	uint8_t reflectivity;
} cdata_t;

typedef struct {
	uint8_t azimuth[2];
	cdata_t channel_data[32];
} dblock_t;

typedef struct {
	uint8_t flag[2];
	dblock_t data;
} fdblock_t;

typedef struct {
	uint8_t header[42];
	fdblock_t data_blocks[12];
	uint8_t timestamp[4];
	uint8_t factory[2];
} packet_t;

#pragma pack(pop)

dblock_t block_read(const unsigned char* block);

#endif