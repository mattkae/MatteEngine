#ifndef CAMERA_H
#define CAMERA_H

#include "GlobalApplicationState.h"
#include "Vector4f.h"
#include "Matrix4x4f.h"
#include "ShaderUniformMapping.h"

struct Camera {
	Vector3f position = { 0, 0, 5 };
	Vector3f up = { 0, 1, 0 };
	Vector3f right = { 1, 0, 0 };
	Vector3f forward = { 0, 0, 1 };

	GLfloat speed = 10.f;
	GLfloat sensitivity = 145.f;

	GLfloat pitch = 0.f;
	GLfloat yaw = 0.f;
	GLfloat maxPitch = 75.f;

	GLfloat fov = 0.872f;

	Matrix4x4f viewMatrix;
	Matrix4x4f projectionMatrix;
};

void updateCamera(Camera& camera, float dt);
Matrix4x4f getCameraViewMatrix(const Camera& camera);
Matrix4x4f getCameraProjection(const Camera& camera);
void renderCamera(const Camera& camera, const CameraUniformMapping& cameraMapping);
#endif
