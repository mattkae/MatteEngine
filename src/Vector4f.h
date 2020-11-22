#pragma once
#include <GL/glew.h>
#include <cmath>
#include <string>

struct Vector4f {
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;
	GLfloat w = 1;

	Vector4f() {};

	Vector4f(GLfloat otherX, GLfloat otherY, GLfloat otherZ, GLfloat otherW) {
		x = otherX;
		y = otherY;
		z = otherZ;
		w = otherW;
	}

	Vector4f(const Vector4f& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	Vector4f(int hexValue) {
		x = ((hexValue >> 16) & 0xFF) / 255.f;  // Extract the RR byte
		y = ((hexValue >> 8) & 0xFF) / 255.f;   // Extract the GG byte
		z = ((hexValue) & 0xFF) / 255.f;        // Extract the BB byte
	}
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

// Operator overloads
Vector4f operator*(const Vector4f& vec, GLfloat value);
Vector4f operator+(const Vector4f& v1, const Vector4f& v2);
Vector4f operator*(const Vector4f& v1, const Vector4f& v2);
