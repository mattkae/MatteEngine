#ifndef PARTICLE_H
#define PARTICLE_H
#include "gl_includes.h"
#include <glm/glm.hpp>

class Shader;

struct Particle {
  glm::vec3 position = glm::vec3(0.0f);
  glm::vec3 color = glm::vec3(1.0f);
  glm::vec3 volumeDimension = glm::vec3(1.0f);
  glm::vec3 particleDimension = glm::vec3(1.0f);
  float density = 1.f;
  int numVertices = 3;

  GLuint vao = 0;
  GLuint vbo = 0;
  GLuint ebo = 0;
};

void initialize_particle(Particle& particle);
void render_particle(const Particle& particle, const Shader& shader);

#endif
