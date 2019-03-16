#ifndef LIDAR_ALGEBRA_MATRIX_3
#define LIDAR_ALGEBRA_MATRIX_3

typedef struct {
	float elements[3][3];
} matrix3vf;

typedef struct {
	float elements[3][2];
} matrix3x2vf;

typedef struct {
	float elements[3][4];
} matrix3x4vf;

#endif