#include "Vector4f.h"
#include "Logger.h"

Vector4f addVector(const Vector4f& first, const Vector4f& second) {
	return {
		first.x + second.x,
		first.y + second.y,
		first.z + second.z,
		first.w + second.w
	};
}

GLfloat dotProduct(const Vector4f& first, const Vector4f& second) {
	return first.x * second.x + first.y * second.y + first.z * second.z + first.w * second.w;
}

GLfloat length(const Vector4f& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

Vector4f scale(const Vector4f&v, GLfloat scalar) {
	return {
		v.x * scalar,
		v.y * scalar,
		v.z * scalar,
		v.w * scalar
	};
}

Vector4f subtract(const Vector4f& first, const Vector4f& second) {
	return {
		first.x - second.x,
		first.y - second.y,
		first.z - second.z,
		first.w - second.w
	};
}

Vector4f negate(const Vector4f& v) {
	return {
		-v.x,
		-v.y,
		-v.z,
		-v.w
	};
}

Vector4f toCartesian(const Vector4f& v) {
	return {
		v.x / v.w,
		v.y / v.w,
		v.z / v.w,
		1.f
	};
}

Vector4f normalize(const Vector4f& v) {
	GLfloat l = length(v);
	return {
		v.x / l,
		v.y / l,
		v.z / l,
		1.f
	};
}

Vector4f cross(const Vector4f& first, const Vector4f& second) {
	return {
		first.y * second.z - first.z * second.y,
		first.z * second.x - first.x * second.z,
		first.x * second.y - first.y * second.x,
		1.f
	};
}

void logVector4f(const Vector4f& v, std::string variableName) {
	Logger::logInfo(variableName + " = (" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ", " + std::to_string(v.w) + ")");
}