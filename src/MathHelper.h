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

	inline int nearestPowerOfTwo(int n) {
        int v = n; 

        v--;
        v |= v >> 1;
        v |= v >> 2;
        v |= v >> 4;
        v |= v >> 8;
        v |= v >> 16;
        v++; // next power of 2

        int x = v >> 1; // previous power of 2

        return (v - n) > (n - x) ? x : v;
    }
}
