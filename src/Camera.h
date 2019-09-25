#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Constants.h"
#include "Shader.h"
#include "Vector4f.h"
#include "Matrix4x4f.h"
#include <glm/glm.hpp>

struct BetterCamera {
	Vector3f position = { 0, 0, 1 };
	Vector3f up = { 0, 1, 0 };
	Vector3f right = { 1, 0, 0 };
	Vector3f forward = { 0, 0, 1 };

	GLfloat speed = 10.f;
	GLfloat sensitivity = 145.f;

	GLfloat pitch = 0.f;
	GLfloat yaw = 0.f;
	GLfloat maxPitch = 75.f;

	GLfloat fov = 0.872f;
};

void updateCamera(BetterCamera& camera, float dt);
Matrix4x4f getCameraViewMatrix(const BetterCamera& camera);
Matrix4x4f getCameraProjection(const BetterCamera& camera);
void renderCamera(const BetterCamera& camera, const Shader& shader, bool withEye = false);
#endif
