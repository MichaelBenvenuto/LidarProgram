#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include <ctime>
#include <chrono>

#include "include/lidar_converter.h"

using namespace std::chrono;

duration<double> func(LidarConverter* conv, int *size) {
	vec3* p = 0;
	col3* c = 0;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	conv->load_raw(&p, &c, size);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	free(p);
	free(c);
	return duration_cast<duration<double>>(t2 - t1);
}

duration<double> func1(LidarConverter* conv, int *size) {
	vec3* p;
	col3* c;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	conv->load_raw1(&p, &c, size);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	free(p);
	free(c);
	return duration_cast<duration<double>>(t2 - t1);
}

duration<double> func2(LidarConverter* conv, int *size) {
	vec3* p;
	col3* c;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	conv->load_raw2(&p, &c, size);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	free(p);
	free(c);
	return duration_cast<duration<double>>(t2 - t1);
}

duration<double> func3(LidarConverter* conv, int *size) {
	vec3* p;
	col3* c;
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	conv->load_raw3(&p, &c, size);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	free(p);
	free(c);
	return duration_cast<duration<double>>(t2 - t1);
}

int main(void) {

	FILE* f[4] = {
	fopen("C:\\Users\\Michael\\Desktop\\VELODYNE\\VLP-16 Sample Data\\2015-07-23-14-37-22_Velodyne-VLP-16-Data_Downtown 10Hz Single.pcap", "rb"),
	fopen("C:\\Users\\Michael\\Desktop\\VELODYNE\\VLP-16 Sample Data\\2015-07-23-14-59-16_Velodyne-VLP-16-Data_Downtown 20Hz Dual.pcap", "rb"),
	fopen("C:\\Users\\Michael\\Desktop\\VELODYNE\\VLP-16 Sample Data\\2015-07-23-15-08-34_Velodyne-VLP-16-Data_Depot 10Hz Single Returns.pcap", "rb"),
	fopen("C:\\Users\\Michael\\Desktop\\VELODYNE\\VLP-16 Sample Data\\2015-07-23-15-14-06_Velodyne-VLP-16-Data_Depot 10Hz Dual Returns.pcap", "rb")
};

	fseek(f[0], 0, SEEK_END);
	fseek(f[1], 0, SEEK_END);
	fseek(f[2], 0, SEEK_END);
	fseek(f[3], 0, SEEK_END);

	int s[4] = {
		ftell(f[0]),
		ftell(f[1]),
		ftell(f[2]),
		ftell(f[3])
	};


	rewind(f[0]);
	rewind(f[1]);
	rewind(f[2]);
	rewind(f[3]);

	FILE* f1 = fopen("C:\\Users\\Michael\\Desktop\\DATA.csv", "w+");

	unsigned char *buf1;

	LidarConverter* lconv = new LidarConverter(NULL, 0);

	duration<double> t;

	int size;

	for (int i = 0; i < 4; i++) {

		buf1 = (unsigned char*)calloc(s[i], sizeof(unsigned char*));
		fread(buf1, sizeof(unsigned char), s[i], f[i]);

		lconv->set_data(buf1, s[i]);

		fprintf(f1, "File %i\n,Time, Data Points\n", i + 1);
		t = func(lconv, &size);
		fprintf(f1, "%i,%f, %i\n", 1, t, size);
		t = func1(lconv, &size);
		fprintf(f1, "%i,%f, %i\n", 2, t, size);
		t = func2(lconv, &size);
		fprintf(f1, "%i,%f, %i\n", 3, t, size);
		t = func3(lconv, &size);
		fprintf(f1, "%i,%f, %i\n", 4, t, size);

		free(buf1);
	}

	fclose(f1);

	return 0;
}