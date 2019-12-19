#include "Ray.h"
Vector4f clickToRay(const BetterCamera& camera) {
	Point cursorPosition = getCursorPosition();
	
	// Most of my understanding of how I was to get the ray from the point clicked
	// on screen comes from here: http://antongerdelan.net/opengl/raycasting.html
	Vector4f ndcPoint;
	ndcPoint.x = (2.f * cursorPosition.x) / GlobalAppState.floatWidth - 1.0f;
	ndcPoint.y = 1.0f - (2.f * (GlobalAppState.floatHeight - cursorPosition.y)) / GlobalAppState.floatHeight;
	ndcPoint.z = 1.0f;
	ndcPoint.w = 1.0f;

	Matrix4x4f inverseProj;
	if (!inverse(getCameraProjection(camera), inverseProj)) {
		return Vector4f();
	}

	Matrix4x4f inverseView;
	if (!inverse(getCameraViewMatrix(camera), inverseView)) {
		return Vector4f();
	}

	Vector4f rayEye = mult(inverseProj, ndcPoint);
	rayEye.z = -1.f;
	rayEye.w = 0.f;
	return normalize(mult(inverseView, rayEye));
}
