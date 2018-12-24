#ifndef PARTICLE_H
#define PARTICLE_H
#include "gl_includes.h"
#include <glm/glm.hpp>
#include <vector>
#include <functional>
#include <queue>

class Shader;
class Camera;

struct Particle {
  glm::vec3 position = glm::vec3(0);;
  glm::vec3 velocity = glm::vec3(0, 1, 0);
  glm::mat4 rotation = glm::mat4(0);

  float timeAlive = 0;
  bool alive = false;
};

struct ParticleEmitter {
  std::vector<Particle> particles;
  std::queue<int> inactiveParticles;

  glm::mat4 model = glm::mat4(1.0f);
  glm::vec3 color = glm::vec3(1.0f);
  glm::vec3 volumeDimension = glm::vec3(1.0f);
  glm::vec3 particleDimension = glm::vec3(1.0f);
  glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 initialVelocity = glm::vec3(0.0, 1.0, 0.0);
  int numParticles = 10;
  int numVertices = 3;

  float particleLife = 15;
  float deltaParticleLife = 0;
  glm::vec2 spawnRange = glm::vec2(0.5f, 2.5f);
  float timeUntilSpawn = 0;
  std::function<glm::vec3(float)> velocityFunction = [](float t) { return glm::vec3(0, t, 0); };

  GLuint vao = 0;
  GLuint vbo = 0;
  GLuint ebo = 0;
  GLenum drawType = GL_TRIANGLE_FAN;
};

void initialize_particle_emitter(ParticleEmitter& particleEmitter);
void update_particle_emitter(ParticleEmitter& particleEmitter, float deltaTime);
void render_particle_emitter(const ParticleEmitter& particleEmitter, Shader& shader, Camera& camera);

#endif
