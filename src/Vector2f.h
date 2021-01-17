#pragma once
#include <GL/glew.h>
#include "Logger.h"

struct Vector2f {
	GLfloat x;
	GLfloat y;

	GLfloat& operator[](int index) {
		if (index == 0) {
			return x;
		} else {
			return y;
		}
	}

	void printDebug(const char* name) const {
		logger_info("%s: (%f, %f)", name, x, y);
	}

};

Vector2f operator+(const Vector2f& v1, const Vector2f& v2);
Vector2f operator+(const Vector2f& vec, GLfloat value);
Vector2f operator-(const Vector2f& vec, GLfloat value);
Vector2f operator-(const Vector2f& v1, const Vector2f& v2);
Vector2f operator*(const Vector2f& vec, GLfloat value);
