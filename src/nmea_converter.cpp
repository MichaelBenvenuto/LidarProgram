#include "nmea_converter.h"

#define _CRT_SECURE_NO_WARNINGS

#include <string.h>
#include <stdlib.h>
#include <math.h>


int conv_NMEA(const char* message, float* latitude, float* longitude) {
	if (strncmp(message, "$GPRMC", 6) != 0) {
		return -1;
	}
	int len = strlen(message);

	int comma_f = 6;
	int comma_l = 6;

	int value = 1;

	float degrees;
	float minutes;

	for (int i = 6; i < len; i++) {
		if (message[i] == ',') {
			comma_l = comma_f;
			comma_f = i;

			int size;
			char* float_conv;

			switch (value) {

			case 2://Timestamp
				break;
			case 3://Validity

				if (message[comma_l + 1] != 'A') {
					return 0;
				}

				break;
			case 4://Latitude

				size = comma_f - comma_l;
				float_conv = (char*)calloc(size, sizeof(char));
				//strncpy(float_conv, &message[comma_l + 1], size - 1);
				strncpy_s(float_conv, size, &message[comma_l + 1], size - 1);
				*latitude = atof(float_conv);

				degrees = floor(*latitude / 100);
				minutes = fmodf(*latitude, 100);

				*latitude = degrees + (minutes / 60);

				free(float_conv);

				break;
			case 5://Heading (N/S)

				if (message[comma_l + 1] == 'S') {
					*latitude = -(*latitude);
				}

				break;
			case 6://Longitude

				size = comma_f - comma_l;
				float_conv = (char*)calloc(size, sizeof(char));
				//strncpy(float_conv, &message[comma_l + 1], size - 1);
				strncpy_s(float_conv, size, &message[comma_l + 1], size - 1);

				*longitude = atof(float_conv);

				degrees = floor(*longitude / 100);
				minutes = fmodf(*longitude, 100);

				*longitude = degrees + (minutes / 60);

				free(float_conv);

				break;
			case 7://Heading (E/W)

				if (message[comma_l + 1] == 'W') {
					*longitude = -(*longitude);
				}

				return 1;
			/*case 8://Speed (Knots)
				break;
			case 9://True course
				break;
			case 10://Date stamp
				break;
			case 11://Variation
				break;
			case 12://Heading (E/W)
				break;
			case 13://Checksum
				break;*/
			}
			value++;
		}
	}

}

