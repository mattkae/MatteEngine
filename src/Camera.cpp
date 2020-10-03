#include "Camera.h"
#include "App.h"
#include "Input.h"
#include "MathHelper.h"
#include "ShaderUniformMapping.h"
#include <GLFW/glfw3.h>
#include <cmath>

const Vector3f WORLD_UP{0.0, 1.0, 0.0};

Matrix4x4f Camera::getView() { 
	return getLookAt(position, position + forward, up);
}

Matrix4x4f Camera::getProjection() {
	return getPerspectiveProjection(GlobalApp.near, GlobalApp.far, fov, GlobalApp.aspectRatio);
}

void Camera::update(float dt) {
	if (isKeyDown(GLFW_KEY_W, DEFAULT_FOCUS_TOKEN)) {
		position = addVector(position, scale(forward, speed * dt));
	}
	if (isKeyDown(GLFW_KEY_A, DEFAULT_FOCUS_TOKEN)) {
		position = subtractVector(position, scale(right, speed * dt));
	}
	if (isKeyDown(GLFW_KEY_S, DEFAULT_FOCUS_TOKEN)) {
		position = subtractVector(position,scale(forward, speed * dt));
	}
	if (isKeyDown(GLFW_KEY_D, DEFAULT_FOCUS_TOKEN)) {
		position = addVector(position, scale(right, speed * dt));
	}
	if (isKeyDown(GLFW_KEY_LEFT, DEFAULT_FOCUS_TOKEN)) {
		yaw -= dt * sensitivity;
	}
	if (isKeyDown(GLFW_KEY_UP, DEFAULT_FOCUS_TOKEN)) {
		pitch += dt * sensitivity;

		if (pitch > maxPitch) {
			pitch = maxPitch;
		}
	}
	if (isKeyDown(GLFW_KEY_RIGHT, DEFAULT_FOCUS_TOKEN)) {
		yaw += dt * sensitivity;
	}
	if (isKeyDown(GLFW_KEY_DOWN, DEFAULT_FOCUS_TOKEN)) {
		pitch -= dt * sensitivity;

		if (pitch < -maxPitch) {
			pitch = -maxPitch;
		}
	}

	GLfloat inverseYaw = yaw - 90;
	forward.x = cos(MathHelper::degreesToRadians(pitch)) * cos(MathHelper::degreesToRadians(inverseYaw));
	forward.y = sin(MathHelper::degreesToRadians(pitch));
	forward.z = cos(MathHelper::degreesToRadians(pitch)) * sin(MathHelper::degreesToRadians(inverseYaw));

	forward = normalize(forward);
	updateMatrices();
}

void Camera::updateMatrices() {
	right = normalize(cross(forward, WORLD_UP));
	up = normalize(cross(right, forward));
	viewMatrix = getView();
	projectionMatrix = getProjection();
}

void Camera::render(const CameraUniformMapping& cameraMapping) const {
	setShaderMat4(cameraMapping.PROJECTION_MATRIX, projectionMatrix);
	setShaderMat4(cameraMapping.VIEW_MATRIX, viewMatrix);
	if (cameraMapping.WITH_EYE) {
		setShaderVec3(cameraMapping.EYE_MATRIX, position);
	}
}
