#ifndef LIDAR_ALGEBRA_MATRIX_4
#define LIDAR_ALGEBRA_MATRIX_4

typedef struct {
	float elements[4][4];
} matrix4vf;

typedef struct {
	float elements[4][3];
} matrix4x3vf;

typedef struct {
	float elements[4][2];
} matrix4x2vf;

#endif