#include "Camera.h"
#include "Input.h"
#include "MathHelper.h"
#include "ShaderUniformMapping.h"
#include <GLFW/glfw3.h>
#include <cmath>

const Vector3f WORLD_UP{0.0, 1.0, 0.0};

inline Matrix4x4f getCameraViewMatrix(const Camera& camera) { 
	return getLookAt(camera.position, camera.position + camera.forward, camera.up);
};

inline Matrix4x4f getCameraProjection(const Camera& camera) {
	return getPerspectiveProjection(GlobalAppState.near, GlobalAppState.far, camera.fov, GlobalAppState.aspectRatio);
};

void updateCamera(Camera& camera, float dt) {
	if (isKeyDown(GLFW_KEY_W, DEFAULT_FOCUS_TOKEN)) {
		camera.position = addVector(camera.position, scale(camera.forward, camera.speed * dt));
	}
	if (isKeyDown(GLFW_KEY_A, DEFAULT_FOCUS_TOKEN)) {
		camera.position = subtractVector(camera.position, scale(camera.right, camera.speed * dt));
	}
	if (isKeyDown(GLFW_KEY_S, DEFAULT_FOCUS_TOKEN)) {
		camera.position = subtractVector(camera.position,scale(camera.forward, camera.speed * dt));
	}
	if (isKeyDown(GLFW_KEY_D, DEFAULT_FOCUS_TOKEN)) {
		camera.position = addVector(camera.position, scale(camera.right, camera.speed * dt));
	}
	if (isKeyDown(GLFW_KEY_LEFT, DEFAULT_FOCUS_TOKEN)) {
		camera.yaw -= dt * camera.sensitivity;
	}
	if (isKeyDown(GLFW_KEY_UP, DEFAULT_FOCUS_TOKEN)) {
		camera.pitch += dt * camera.sensitivity;

		if (camera.pitch > camera.maxPitch) {
			camera.pitch = camera.maxPitch;
		}
	}
	if (isKeyDown(GLFW_KEY_RIGHT, DEFAULT_FOCUS_TOKEN)) {
		camera.yaw += dt * camera.sensitivity;
	}
	if (isKeyDown(GLFW_KEY_DOWN, DEFAULT_FOCUS_TOKEN)) {
		camera.pitch -= dt * camera.sensitivity;

		if (camera.pitch < -camera.maxPitch) {
			camera.pitch = -camera.maxPitch;
		}
	}

	Vector3f forwardTemp;
	GLfloat pitch = camera.pitch;
	GLfloat yaw = camera.yaw - 90.f;
	forwardTemp.x = cos(MathHelper::degreesToRadians(pitch)) * cos(MathHelper::degreesToRadians(yaw));
	forwardTemp.y = sin(MathHelper::degreesToRadians(pitch));
	forwardTemp.z = cos(MathHelper::degreesToRadians(pitch)) * sin(MathHelper::degreesToRadians(yaw));

	camera.forward = normalize(forwardTemp);
	camera.right = normalize(cross(camera.forward, WORLD_UP));
	camera.up = normalize(cross(camera.right, camera.forward));

	camera.viewMatrix = getCameraViewMatrix(camera);
	camera.projectionMatrix = getCameraProjection(camera);
}

void renderCamera(const Camera& camera, const CameraUniformMapping& cameraMapping) {
	setShaderMat4(cameraMapping.PROJECTION_MATRIX, camera.projectionMatrix);
	setShaderMat4(cameraMapping.VIEW_MATRIX, camera.viewMatrix);
	if (cameraMapping.WITH_EYE) {
		setShaderVec3(cameraMapping.EYE_MATRIX, camera.position);
	}
}