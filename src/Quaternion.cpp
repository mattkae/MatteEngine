#include "Quaternion.h"

Quaternion Quaternion::operator*(const Quaternion& other) {
	return {
		w * other.w - x * other.x - y * other.y - z * other.z,  // 1
        w * other.x + x * other.w + y * other.z - z * other.y,  // i
        w * other.y - x * other.z + y * other.w + z * other.x,  // j
        w * other.z + x * other.y - y * other.x + z * other.w   // k
	};
}

GLfloat Quaternion::length() const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

Quaternion Quaternion::normalize() const {
	GLfloat l = length();
	return {
		1.f,
		x / l,
		y / l,
		z / l,
	};
}

Matrix4x4f Quaternion::toMatrix() const {
	return {
		{
			1 - 2 * (y * y +  z * z), 
			2 * (x * y + z * w),
			2 * (x * z - y * w),
			0,
			2 * (x * y - w * z),
			1 - 2 * (x * x + z * z),
			2 * (y * z + w * x),
			0,
			2 * (x * z + w * y),
			2 * (y * z - w * x),
			1 - 2 * (x * x + y * y),
			1
		}
	};
}