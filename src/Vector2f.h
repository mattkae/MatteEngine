#pragma once
#include <GL/glew.h>

struct Vector2f {
	GLfloat x;
	GLfloat y;
};

Vector2f operator+(const Vector2f& v1, const Vector2f& v2);
Vector2f operator+(const Vector2f& vec, GLfloat value);
Vector2f operator-(const Vector2f& vec, GLfloat value);
Vector2f operator-(const Vector2f& v1, const Vector2f& v2);
Vector2f operator*(const Vector2f& vec, GLfloat value);