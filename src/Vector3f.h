#pragma once
#include <GL/glew.h>
#include <cmath>
#include "Vector4f.h"

enum UnitDirectionVector {
	X = 0,
	Y = 1,
	Z = 2
};

struct Vector3f {
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;
	GLfloat operator [](int index);
	Vector3f interpolate(const Vector3f& other, const GLfloat factor);
	Vector3f reflect(const Vector3f& other);
	void printDebug(const char* name = "") const;
};

// Creation
Vector3f getVec3(GLfloat value);
Vector3f getVec3(GLfloat x, GLfloat y, GLfloat z);

// Methods
Vector3f addVector(const Vector3f& first, const Vector3f& second);
GLfloat dotProduct(const Vector3f& first, const Vector3f& second);
GLfloat length(const Vector3f& v);
Vector3f scale(const Vector3f&v, GLfloat scalar);
Vector3f subtractVector(const Vector3f& first, const Vector3f& second);
Vector3f subtractVector(const Vector3f& first, const Vector4f& second);
Vector3f negate(const Vector3f& v);
Vector3f cross(const Vector3f& first, const Vector3f& second);
Vector3f normalize(const Vector3f& v);
Vector3f fromVec4(const Vector4f& v);

// Operator overlaods
Vector3f operator+(const Vector3f& v1, const Vector3f& v2);
Vector3f operator-(const Vector3f& v1, const Vector3f& v2);
Vector3f operator-(const Vector3f& v1,const  Vector4f& v2);
Vector3f operator-(const Vector3f& vec);
Vector3f operator*(const Vector3f& vec, GLfloat value);
Vector3f operator*(const Vector3f& v1, const Vector3f& v2);
