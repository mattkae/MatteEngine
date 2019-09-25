#include "Vector3f.h"

Vector3f addVector(const Vector3f& first, const Vector3f& second) {
	return {
		first.x + second.x,
		first.y + second.y,
		first.z + second.z
	};
}

GLfloat dotProduct(const Vector3f& first, const Vector3f& second) {
	return first.x * second.x + first.y * second.y + first.z * second.z;
}

GLfloat length(const Vector3f& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3f scale(const Vector3f&v, GLfloat scalar) {
	return {
		v.x * scalar,
		v.y * scalar,
		v.z * scalar
	};
}

Vector3f subtractVector(const Vector3f& first, const Vector3f& second) {
	return {
		first.x - second.x,
		first.y - second.y,
		first.z - second.z
	};
}

Vector3f negate(const Vector3f& v) {
	return {
		-v.x,
		-v.y,
		-v.z
	};
}

Vector3f cross(const Vector3f& first, const Vector3f& second) {
	return {
		first.y * second.z - first.z * second.y,
		first.z * second.x - first.x * second.z,
		first.x * second.y - first.y * second.x
	};
}

Vector3f normalize(const Vector3f& v) {
	GLfloat len = 1.f / length(v);
	return {
		v.x * len,
		v.y * len,
		v.z * len
	};
}