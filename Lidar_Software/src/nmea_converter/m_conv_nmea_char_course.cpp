#include "h_nmea_converter_common.h"

#include <stdio.h>

int conv_NMEA(const char* message, double* latitude, double* longitude, double* t_course) {
	if (strncmp(message, "$GPRMC", 6) != 0) {
		return -1;
	}
	int len = strlen(message);

	int comma_f = 6;
	int comma_l = 6;

	int value = 1;

	double degrees;
	double minutes;

	double variation;

	for (int i = 6; i < len; i++) {
		if (message[i] == ',') {
			comma_l = comma_f;
			comma_f = i;

			int size;
			char* double_conv = 0;

			switch (value) {

			case 2://Timestamp
				break;
			case 3://Validity

				if (message[comma_l + 1] != 'A') {
					printf("INVALID\n");
					return 0;
				}

				break;
			case 4://Latitude

				size = comma_f - comma_l;
				double_conv = (char*)calloc(size, sizeof(char));
				//strncpy(double_conv, &message[comma_l + 1], size - 1);
				strncpy_s(double_conv, size, &message[comma_l + 1], (size_t)(size)-1ull);
				*latitude = atof(double_conv);

				degrees = floor(*latitude / 100);
				minutes = fmodf(*latitude, 100);

				*latitude = degrees + (minutes / 60);

				free(double_conv);

				break;
			case 5://Heading (N/S)

				if (message[comma_l + 1] == 'S') {
					*latitude = -(*latitude);
				}

				break;
			case 6://Longitude

				size = comma_f - comma_l;
				double_conv = (char*)calloc(size, sizeof(char));
				//strncpy(double_conv, &message[comma_l + 1], size - 1);
				strncpy_s(double_conv, size, &message[comma_l + 1], (size_t)(size)-1ull);

				*longitude = atof(double_conv);

				degrees = floor(*longitude / 100);
				minutes = fmodf(*longitude, 100);

				*longitude = degrees + (minutes / 60);

				free(double_conv);

				break;
			case 7://Heading (E/W)

				if (message[comma_l + 1] == 'W') {
					*longitude = -(*longitude);
				}
				case 8://Speed (Knots)
				break;
				case 9://True course
					size = comma_f - comma_l;
					double_conv = (char*)calloc(size, sizeof(char));
					strncpy_s(double_conv, size, &message[comma_l + 1], (size_t)(size)-1ull);

					*t_course = atof(double_conv);

					free(double_conv);
					return 1;
				break;
				case 10://Date stamp
				break;
				case 11://Variation

					size = comma_f - comma_l;
					double_conv = (char*)calloc(size, sizeof(char));
					strncpy_s(double_conv, size, &message[comma_l + 1], (size_t)(size)-1ull);

					variation = atof(double_conv);

					free(double_conv);
				break;
				case 12://Heading (E/W)

					if (message[comma_l + 1] == 'E') {
						variation = -variation;
					}

					(*t_course) -= variation;

					//printf("%f\n", *t_course);

					return 1;
			}
			value++;
		}
	}
	return 0;
}