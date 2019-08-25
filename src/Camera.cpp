#define GLM_ENABLE_EXPERIMENTAL

#include "Camera.h"
#include "Input.h"
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

const glm::vec3 WORLD_UP(0.0, 1.0, 0.0);
extern int primaryFocusToken;

void updateCamera(BetterCamera& camera, float dt) {
	if (isKeyDown(GLFW_KEY_W, primaryFocusToken)) {
		camera.position += dt * camera.speed * camera.forward;
	}
	if (isKeyDown(GLFW_KEY_A, primaryFocusToken)) {
		camera.position -= dt * camera.speed * camera.right;
	}
	if (isKeyDown(GLFW_KEY_S, primaryFocusToken)) {
		camera.position -= dt * camera.speed * camera.forward;
	}
	if (isKeyDown(GLFW_KEY_D, primaryFocusToken)) {
		camera.position += dt * camera.speed * camera.right;
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

	glm::vec3 frontTemp;
	float pitch = camera.pitch;
	float yaw = camera.yaw - 90.f;
	frontTemp.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	frontTemp.y = sin(glm::radians(pitch));
	frontTemp.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	camera.forward = glm::normalize(frontTemp);
	camera.right = glm::normalize(glm::cross(camera.forward, WORLD_UP));
	camera.up = glm::normalize(glm::cross(camera.right, camera.forward));
}

void renderCamera(const BetterCamera& camera, const Shader& shader, bool withEye) {
	setShaderMat4(shader, "uVp", getCameraProjection(camera) * getCameraViewMatrix(camera));
	if (withEye) {
		setShaderVec3(shader, "uEye", camera.position);
	}
}