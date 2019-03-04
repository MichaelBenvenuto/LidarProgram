#ifndef LIDAR_PDATA
#define LIDAR_PDATA

#include <stdint.h>

#pragma pack(push, 1)
typedef struct cdata_t {
	uint8_t distance[2];
	uint8_t reflectivity;
};

typedef struct dblock_t {
	uint8_t azimuth[2];
	cdata_t channel_data[32];
};

typedef struct fdblock_t {
	uint8_t flag[2];
	dblock_t data;
};

typedef struct packet_t {
	uint8_t header[42];
	fdblock_t data_blocks[12];
	uint8_t timestamp[4];
	uint8_t factory[2];
};

#pragma pack(pop)

dblock_t block_read(const unsigned char* block);

#endif