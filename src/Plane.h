#pragma once
#include "Vector3f.h"
#include "Ray.h"
#include "float.h"

struct Plane {
	Vector3f center;
	Vector3f normal;
};

inline bool isPointOnPlane(const Plane& plane, const Vector3f& point) {
	return abs(dotProduct(subtractVector(point, plane.center), plane.normal)) < FLT_EPSILON;
}

inline bool tryGetRayPointOfIntersection(const Plane& plane, const Ray& ray, Vector3f& result) {
	float denominator = dotProduct(plane.normal, ray.direction);
	if (abs(denominator) > FLT_EPSILON) {
		Vector3f diff = subtractVector(plane.center, ray.position);
		float tVal = dotProduct(diff, plane.normal) / denominator;

		if (tVal > FLT_EPSILON) {
			result = addVector(ray.position, scale(ray.direction, tVal));
			return true;
		}
	}

	return false;
}
