#pragma once
#include <GL/glew.h>
#include "Matrix4x4f.h"

struct Quaternion {
	GLfloat w;
	GLfloat x;
	GLfloat y;
	GLfloat z;

	Quaternion operator*(const Quaternion& other);
};