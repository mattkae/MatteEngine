#include "Quaternion.h"

Quaternion Quaternion::operator*(const Quaternion& other) {
	return {
		w * other.w - x * other.x - y * other.y - z * other.z,  // 1
        w * other.x + x * other.w + y * other.z - z * other.y,  // i
        w * other.y - x * other.z + y * other.w + z * other.x,  // j
        w * other.z + x * other.y - y * other.x + z * other.w   // k
	};
}