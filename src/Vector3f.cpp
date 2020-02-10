#include "Vector3f.h"

Vector3f getVec3(GLfloat value) {
	return { value, value, value };
}

Vector3f getVec3(GLfloat x, GLfloat y, GLfloat z) {
	return { x, y, z };
}

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

Vector3f subtractVector(const Vector3f& first, const Vector4f& second) {
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

Vector3f fromVec4(const Vector4f& v) {
	return {
		v.x,
		v.y,
		v.z
	};
}

Vector3f operator+(const Vector3f& v1, const Vector3f& v2) {
	return addVector(v1, v2);
}

Vector3f operator-(const Vector3f& v1, const Vector3f& v2) {
	return subtractVector(v1, v2);
}

Vector3f operator-(const Vector3f& v1,const  Vector4f& v2) {
	return subtractVector(v1, v2);
}

Vector3f operator-(const Vector3f& vec) {
	return negate(vec);
}

Vector3f operator*(const Vector3f& vec, GLfloat value) {
	return scale(vec, value);
}

Vector3f operator*(const Vector3f& v1, const Vector3f& v2) {
	return {
		v1.x * v2.x,
		v1.y * v2.y,
		v1.z * v2.z
	};
}