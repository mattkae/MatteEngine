#pragma once
#include <GL/glew.h>
#include "Matrix4x4f.h"

struct Quaternion {
	GLfloat w = 0;
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;

	Quaternion operator*(const Quaternion& other);
	Matrix4x4f toMatrix() const;
	Quaternion normalize() const;
	GLfloat length() const;
};