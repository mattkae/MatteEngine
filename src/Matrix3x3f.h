#pragma once
#include <GL/glew.h>
#include "Matrix4x4f.h"

struct Matrix3x3f {
	GLfloat values[9] = {0};
};

Matrix3x3f fromMat4(const Matrix4x4f& matrix);