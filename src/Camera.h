#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Constants.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Shader;

struct BetterCamera {
	glm::vec3 position = glm::vec3(0);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 right = glm::vec3(1, 0, 0);
	glm::vec3 forward = glm::vec3(0, 0, 1);
	
	float speed = 10.f;
	float sensitivity = 145.f;

	float pitch = 0.f;
	float yaw = 0.f;
	float maxPitch = 75.f;

	float fov = 60.f;
};

inline glm::mat4 getCameraViewMatrix(const BetterCamera& camera) { return glm::lookAt(camera.position, camera.position + camera.forward, camera.up);};
inline glm::mat4 getCameraProjection(const BetterCamera& camera) { return glm::perspective(glm::radians(camera.fov), Constants.aspectRatio, Constants.near, Constants.far); };
void updateCamera(BetterCamera& camera, float dt);
void renderCamera(const BetterCamera& camera, const Shader& shader, bool withEye = false);
#endif
