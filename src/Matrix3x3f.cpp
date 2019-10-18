#include "Matrix3x3f.h"

Matrix3x3f fromMat4(const Matrix4x4f& matrix) {
	return {
		{
			matrix.values[0], matrix.values[1], matrix.values[2],
			matrix.values[4], matrix.values[5], matrix.values[6],
			matrix.values[8], matrix.values[9], matrix.values[10]
		}
	};
}