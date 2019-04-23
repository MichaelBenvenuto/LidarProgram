#ifndef LIDAR_CONVERTER_CLASS_COMMON_METHODS
#define LIDAR_CONVERTER_CLASS_COMMON_METHODS

#include "include/lidar_converter.h"
#include "include/pcap_headers/global_header.h"
#include "include/pcap_headers/packet_header.h"
#include "include/packet_data.h"

#include <stdlib.h>
#include <math.h>

const int lookup_table[16] = { -1500,100,-1300,-300,-1100,500,-900,700,-700,900,-500,1100,-300,1300,-100,1500 };

#endif