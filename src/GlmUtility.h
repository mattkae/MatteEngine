#ifndef GLMUTILITY_H
#define GLMUTILITY_H

#include <glm/glm.hpp>

namespace glm {
glm::vec3 get_random_within(glm::vec3 origin, glm::vec3 size);
float get_random_between(glm::vec2 range);
void print(glm::vec3 vector);
void print(glm::vec4 vector);
void print(glm::mat3 matrix);
void print(glm::mat4 matrix);
}

#endif
