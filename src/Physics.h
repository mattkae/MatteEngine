#ifndef PHYSICS_H
#define PHYSICS_H
#include <glm/glm.hpp>

struct PhysicsValues {
  double mass;
  double velocity;
};

glm::mat4 applyGravity(double deltaTime, glm::mat4 previous);

#endif
