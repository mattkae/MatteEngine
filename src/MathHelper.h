#pragma once

#define PI 3.14159265358979323846f

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define CLAMP(x, upper, lower) (MIN(upper, MAX(x, lower)))
#define DTOR (PI / 180.0f)

namespace MathHelper {
	inline float radiansToDegrees(float radians) {
		return radians * 180.f / PI;
	}

	inline float degreesToRadians(float degrees) {
		return degrees * PI / 180.f;
	}
}
