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

// Creation
Vector4f getVec4(GLfloat value);
Vector4f getVec4(GLfloat x, GLfloat y, GLfloat z);
Vector4f getVec4(GLfloat x, GLfloat y, GLfloat z, GLfloat w);

// Methods
Vector4f addVector(const Vector4f& first, const Vector4f& second);
GLfloat dotProduct(const Vector4f& first, const Vector4f& second);
GLfloat length(const Vector4f& v);
Vector4f scale(const Vector4f&v, GLfloat scalar);
Vector4f subtract(const Vector4f& first, const Vector4f& second);
Vector4f negate(const Vector4f& v);
Vector4f toCartesian(const Vector4f& v);
Vector4f normalize(const Vector4f& v);
Vector4f cross(const Vector4f& first, const Vector4f& second);

// Debug
void logVector4f(const Vector4f& v, std::string variableName);

// Operator overloads
Vector4f operator*(const Vector4f& vec, GLfloat value);
Vector4f operator+(const Vector4f& v1, const Vector4f& v2);
Vector4f operator*(const Vector4f& v1, const Vector4f& v2);
