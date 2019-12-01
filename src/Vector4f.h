#pragma once
#include <GL/glew.h>
#include <cmath>
#include <string>

struct Vector4f {
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;
	GLfloat w = 1;
};

Vector4f addVector(const Vector4f& first, const Vector4f& second);
GLfloat dotProduct(const Vector4f& first, const Vector4f& second);
GLfloat length(const Vector4f& v);
Vector4f scale(const Vector4f&v, GLfloat scalar);
Vector4f subtract(const Vector4f& first, const Vector4f& second);
Vector4f negate(const Vector4f& v);
Vector4f toCartesian(const Vector4f& v);
Vector4f normalize(const Vector4f& v);
Vector4f cross(const Vector4f& first, const Vector4f& second);
void logVector4f(const Vector4f& v, std::string variableName);