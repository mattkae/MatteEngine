#pragma once
#include <GL/glew.h>

struct Vector2f {
	GLfloat x;
	GLfloat y;
};

inline Vector2f operator+(const Vector2f& v1, const Vector2f& v2) {
	return addVector2f(v1, v2);
}

inline Vector2f addVector2f(const Vector2f& v1, const Vector2f& v2) {
	return {
		v1.x + v2.x,
		v1.y + v2.y
	};
}