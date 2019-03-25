#ifndef LIDAR_MATH_CURVE_GRAPH
#define LIDAR_MATH_CURVE_GRAPH

struct curve_t{
	int point_count;
	double* from;
	double* to;
};


int add_point(curve_t curve, double from, double to);
int add_point(curve_t curve, double from, double to, int index);

int remove_point(curve_t curve);
int remove_point(curve_t curve, int index);

double calculate_curve(curve_t curve, double input);

#endif