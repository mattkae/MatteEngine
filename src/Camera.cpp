#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

using namespace std;
const Vector3f WORLD_UP{0.0, 1.0, 0.0};
extern int primaryFocusToken;

void updateCamera(BetterCamera& camera, float dt) {
	if (isKeyDown(GLFW_KEY_W, primaryFocusToken)) {
		camera.position = addVector(camera.position, scale(camera.forward, camera.speed * dt));
	}
	if (isKeyDown(GLFW_KEY_A, primaryFocusToken)) {
		camera.position = subtractVector(camera.position, scale(camera.right, camera.speed * dt));
	}
	if (isKeyDown(GLFW_KEY_S, primaryFocusToken)) {
		camera.position = subtractVector(camera.position,scale(camera.forward, camera.speed * dt));
	}
	if (isKeyDown(GLFW_KEY_D, primaryFocusToken)) {
		camera.position = addVector(camera.position, scale(camera.right, camera.speed * dt));
	}
	if (isKeyDown(GLFW_KEY_LEFT, primaryFocusToken)) {
		camera.yaw -= dt * camera.sensitivity;
	}
	if (isKeyDown(GLFW_KEY_UP, primaryFocusToken)) {
		camera.pitch += dt * camera.sensitivity;

		if (camera.pitch > camera.maxPitch) {
			camera.pitch = camera.maxPitch;
		}
	}
	if (isKeyDown(GLFW_KEY_RIGHT, primaryFocusToken)) {
		camera.yaw += dt * camera.sensitivity;
	}
	if (isKeyDown(GLFW_KEY_DOWN, primaryFocusToken)) {
		camera.pitch -= dt * camera.sensitivity;

		if (camera.pitch < -camera.maxPitch) {
			camera.pitch = -camera.maxPitch;
		}
	}

	Vector3f forwardTemp;
	GLfloat pitch = camera.pitch;
	GLfloat yaw = camera.yaw - 90.f;
	forwardTemp.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	forwardTemp.y = sin(glm::radians(pitch));
	forwardTemp.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	camera.forward = normalize(forwardTemp);
	camera.right = normalize(cross(camera.forward, WORLD_UP));
	camera.up = normalize(cross(camera.right, camera.forward));
}

inline Matrix4x4f getCameraViewMatrix(const BetterCamera& camera) { 
	return getLookAt(camera.position, addVector(camera.position, camera.forward), camera.up);
};

inline Matrix4x4f getCameraProjection(const BetterCamera& camera) {
	int width = 0;
	int height = 0;
	getWindowDimensions(width, height);
	GLfloat aspectRatio = static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
	return getProjection(Constants.near, Constants.far, camera.fov, aspectRatio);
};

void renderCamera(const BetterCamera& camera, const Shader& shader, bool withEye) {
	auto view = getCameraViewMatrix(camera);
	auto proj = getCameraProjection(camera);
	auto uVp = mult(view, proj);

	setShaderMat4(shader, "uVp", uVp);
	if (withEye) {
		setShaderVec3(shader, "uEye", camera.position);
	}
}