#include "h_packet_data_common.h"

dblock_t block_read(const unsigned char* block) {
	dblock_t dblock;
	if (block[0] == 0xff && block[1] == 0xee) {
		memcpy(&dblock, block + 2, 98);
	}

	return dblock;
}