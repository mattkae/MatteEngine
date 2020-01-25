#include "Vector2f.h"

Vector2f operator+(const Vector2f& v1, const Vector2f& v2) {
	return {
		v1.x + v2.x,
		v1.y + v2.y
	};
}

Vector2f operator+(const Vector2f& vec, GLfloat value) {
	return {
		vec.x + value,
		vec.y + value
	};
}

Vector2f operator-(const Vector2f& vec, GLfloat value) {
	return {
		vec.x - value,
		vec.y - value
	};
}

Vector2f operator-(const Vector2f& v1, const Vector2f& v2) {
	return {
		v1.x - v2.x,
		v1.y - v2.y
	};
}

Vector2f operator*(const Vector2f& vec, GLfloat value) {
	return {
		vec.x * value,
		vec.y * value
	};
}