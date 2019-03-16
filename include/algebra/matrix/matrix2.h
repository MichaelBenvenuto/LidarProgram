#ifndef LIDAR_ALGEBRA_MATRIX_2
#define LIDAR_ALGEBRA_MATRIX_2

#include "matrix3.h"
#include "matrix4.h"

struct matrix2x3vf;
struct matrix2x4vf;

struct matrix2vf{
	float elements[2][2];

	matrix2vf operator*(float f);
	matrix2vf operator*(matrix2vf b);
	matrix2x3vf operator*(matrix2x3vf b);
	matrix2x4vf operator*(matrix2x4vf b);

	void operator*=(float f);
	void operator*=(matrix2vf b);

	matrix2vf operator+(matrix2vf b);
	matrix2vf operator-(matrix2vf b);

	void operator+=(matrix2vf b);
	void operator-=(matrix2vf b);

	matrix2vf identity();
};

struct matrix2x3vf{
	float elements[2][3];

	matrix2vf operator*(float f);
	matrix2x3vf operator*(matrix3vf b);
	matrix2x3vf operator*(matrix3x2vf b);
	matrix2x4vf operator*(matrix3x4vf b);

	void operator*=(float f);
	void operator*=(matrix2x3vf b);

	matrix2vf operator+(matrix2x3vf b);
	matrix2vf operator-(matrix2x3vf b);

	void operator+=(matrix2x3vf b);
	void operator-=(matrix2x3vf b);

	matrix2vf identity();
};

struct matrix2x4vf{
	float elements[2][4];
};

#endif