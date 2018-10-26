#define GLM_ENABLE_EXPERIMENTAL

#include "Physics.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GlmUtility.h";
#define GRAVITY 9.8

glm::mat4 applyGravity(double deltaTime, glm::mat4 previous) {
  glm::mat4 result = glm::translate(previous, glm::vec3(0, -GRAVITY * deltaTime, 0));
  return result;
}
