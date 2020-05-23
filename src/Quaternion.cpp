#include "Quaternion.h"

GLfloat Quaternion::operator [](int index) {
	switch (index) {
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	case 3:
		return w;
	}
}

Quaternion Quaternion::operator*(const Quaternion& other) const {
	return {
		w * other.w - x * other.x - y * other.y - z * other.z,  // w
        w * other.x + x * other.w + y * other.z - z * other.y,  // i
        w * other.y - x * other.z + y * other.w + z * other.x,  // j
        w * other.z + x * other.y - y * other.x + z * other.w   // k
	};
}

Quaternion Quaternion::operator*(const GLfloat& scale) const {
	return {
		w * scale,
		x * scale,
		y * scale,
		z * scale
	};
}

Quaternion Quaternion::operator+(const Quaternion& other) const {
	return {
		w + other.w,
		x + other.x,
		y + other.y,
		z + other.z
	};
}

Quaternion Quaternion::operator-(const Quaternion& other) const {
	return {
		w - other.w,
		x - other.x,
		y - other.y,
		z - other.z
	};
}

const GLfloat DOT_THRESHOLD = 0.9999; 

// Using a slerp here, mostly taken from here: http://number-none.com/product/Understanding%20Slerp,%20Then%20Not%20Using%20It/.
// As JBlow says, it's expensive as heck. I will address this if it becomes a problem.
Quaternion Quaternion::interpolate(const Quaternion& other, const GLfloat factor) {
	Quaternion newOther = other;
	GLfloat dotProduct = dot(other);

	if (dotProduct < 0) {
		newOther = other * -1;
		dotProduct *= -1;
	}

	if (dotProduct > DOT_THRESHOLD) {
		return (*this + ((newOther - *this) * factor)).normalize();
	}

	GLfloat thetaZero = acos(dotProduct); // angle between input vectors
	GLfloat theta = thetaZero * factor;

	Quaternion v2 = (newOther - (*this * dotProduct)).normalize();

	return (*this * cos(theta)) + (v2 * sin(theta));
}

GLfloat Quaternion::length() const {
	return sqrtf(x * x + y * y + z * z + w * w);
}

Quaternion Quaternion::normalize() const {
	GLfloat l = length();
	return {
		w / l,
		x / l,
		y / l,
		z / l,
	};
}

Matrix4x4f Quaternion::toMatrix() const {
	return {
		{
			1 - 2 * (y * y +  z * z), 
			2 * (x * y + z * w),
			2 * (x * z - y * w),
			0,
			2 * (x * y - w * z),
			1 - 2 * (x * x + z * z),
			2 * (y * z + w * x),
			0,
			2 * (x * z + w * y),
			2 * (y * z - w * x),
			1 - 2 * (x * x + y * y),
			0,
			0,
			0,
			0,
			1
		}
	};
}

GLfloat Quaternion::dot(const Quaternion& second) const {
	return w * second.w + x * second.x + y * second.y + z * second.z;
}