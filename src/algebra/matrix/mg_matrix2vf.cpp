#include "h_matrix_common.h"

matrix2vf matrix2vf::operator*(float f) {
	matrix2vf matrix;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			matrix.elements[i][j] = this->elements[i][j] * f;
		}
	}
	return matrix;
}

void matrix2vf::operator*=(float f) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			this->elements[i][j] *= f;
		}
	}
}



matrix2vf matrix2vf::operator*(matrix2vf b) {
	matrix2vf matrix;
	
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			matrix.elements[i][j] = 0;
			for (int r = 0; r < 2; r++) {
				matrix.elements[i][j] += this->elements[i][r] * b.elements[r][j];
			}
		}
	}

	return matrix;
}

void matrix2vf::operator*=(matrix2vf b) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			float a = 0;
			for (int r = 0; r < 2; r++) {
				a += this->elements[i][r] * b.elements[r][j];
			}
			this->elements[i][j] = a;
		}
	}
}



matrix2x3vf matrix2vf::operator*(matrix2x3vf b) {
	matrix2x3vf matrix;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			matrix.elements[i][j] = 0;
			for (int r = 0; r < 2; r++) {
				matrix.elements[i][j] += this->elements[i][r] * b.elements[r][j];
			}
		}
	}

	return matrix;
}



matrix2x4vf matrix2vf::operator*(matrix2x4vf b) {
	matrix2x4vf matrix;

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 4; j++) {
			matrix.elements[i][j] = 0;
			for (int r = 0; r < 2; r++) {
				matrix.elements[i][j] += this->elements[i][r] * b.elements[r][j];
			}
		}
	}

	return matrix;
}



matrix2vf matrix2vf::operator+(matrix2vf b) {
	matrix2vf matrix;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			matrix.elements[i][j] = this->elements[i][j] + b.elements[i][j];
		}
	}
	return matrix;
}

void matrix2vf::operator+=(matrix2vf b) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			this->elements[i][j] += b.elements[i][j];
		}
	}
}



matrix2vf matrix2vf::operator-(matrix2vf b) {
	matrix2vf matrix;
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			matrix.elements[i][j] = this->elements[i][j] - b.elements[i][j];
		}
	}
	return matrix;
}

void matrix2vf::operator-=(matrix2vf b) {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			this->elements[i][j] -= b.elements[i][j];
		}
	}
}

matrix2vf matrix2vf::identity() {
	matrix2vf i = {
		1.0, 0.0,
		0.0, 1.0
	};
	return *this * i;
}