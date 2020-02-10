#pragma once

#define PI 3.14159265358979323846f

namespace MathHelper {
	inline float radiansToDegrees(float radians) {
		return radians * 180.f / PI;
	}

	inline float degreesToRadians(float degrees) {
		return degrees * PI / 180.f;
	}
}