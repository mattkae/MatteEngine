#pragma once
#include <GL/glew.h>
#include "Matrix4x4f.h"

struct Quaternion {
	GLfloat w = 1.f;
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;

	GLfloat* operator [](int index);
	Quaternion operator*(const Quaternion& other) const;
	Quaternion operator*(const GLfloat& scale) const;
	Quaternion operator+(const Quaternion& other) const;
	Quaternion operator-(const Quaternion& other) const;
	Quaternion interpolate(const Quaternion& other, const GLfloat factor);
	Matrix4x4f toMatrix() const;
	Quaternion normalize() const;
	GLfloat length() const;
	GLfloat dot(const Quaternion& other) const;
};