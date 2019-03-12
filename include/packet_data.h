#ifndef LIDAR_PDATA
#define LIDAR_PDATA

#include <stdint.h>

#pragma pack(push, 1)
typedef struct {
	uint16_t distance;
	uint8_t reflectivity;
} cdata_t;

typedef struct {
	uint16_t azimuth;
	cdata_t channel_data[32];
} dblock_t;

typedef struct {
	uint16_t flag;
	dblock_t data;
} fdblock_t;

typedef struct {
	uint8_t header[42];
	fdblock_t data_blocks[12];
	uint32_t timestamp;
	uint16_t factory;
} packet_t;

#pragma pack(pop)

dblock_t block_read(const unsigned char* block);

#endif