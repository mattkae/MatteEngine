#include "Particle.h"
#include "Shader.h"
#include "Camera.h"
#include "GlmUtility.h"
#include <vector>
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

#define PI 3.14159265

void initialize_particle_emitter(ParticleEmitter& particleEmitter) {
  glGenVertexArrays(1, &particleEmitter.vao);
  glGenBuffers(1, &particleEmitter.vbo);
  glGenBuffers(1, &particleEmitter.ebo);

  // Times two for vertex position + color
  int verticesCount = 2 * 3 * particleEmitter.numVertices;
  GLfloat* vertices = new GLfloat[verticesCount];
  float theta = (360.f / particleEmitter.numVertices);

  // Vertices
  int index = 0;
  for (int vidx = 0; vidx < particleEmitter.numVertices; vidx++) {
    float thetaRadians = ((theta * vidx) * PI) / 180.f;
    vertices[index] = cos(thetaRadians) * particleEmitter.particleDimension.x;
    vertices[index + 1] = sin(thetaRadians) * particleEmitter.particleDimension.y;
    vertices[index + 2] = 1.0f;
    vertices[index + 3] = particleEmitter.color.x;
    vertices[index + 4] = particleEmitter.color.y;
    vertices[index + 5] = particleEmitter.color.z;
    index += 6;
  }

  // Indices
  GLuint* indices = new GLuint[particleEmitter.numVertices];
  for (int iidx = 0; iidx < particleEmitter.numVertices; iidx++) {
    indices[iidx] = iidx;
  }
  
  glBindVertexArray(particleEmitter.vao);
  
  glBindBuffer(GL_ARRAY_BUFFER, particleEmitter.vbo);
  glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particleEmitter.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, particleEmitter.numVertices * sizeof(GLuint), indices, GL_STATIC_DRAW);
  
  // Position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);

  // Color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  
  glBindVertexArray(0);

  delete vertices;
  delete indices;

  particleEmitter.particles.resize(particleEmitter.numParticles);
  for (int pidx = 0; pidx < particleEmitter.numParticles; pidx++) {
    particleEmitter.inactiveParticles.push(pidx);
  }
}

void update_particle_emitter(ParticleEmitter& particleEmitter, float deltaTime) {
  // Try spawn new particle
  particleEmitter.timeUntilSpawn -= deltaTime;
  if (particleEmitter.timeUntilSpawn <= 0 && particleEmitter.inactiveParticles.size() != 0) {
    int pidx = particleEmitter.inactiveParticles.front();
    particleEmitter.inactiveParticles.pop();
    Particle& particle = particleEmitter.particles[pidx];
    particle.velocity = particleEmitter.initialVelocity;
    particle.position = glm::get_random_within(particleEmitter.origin, particleEmitter.volumeDimension);
    particle.alive = true;
    particleEmitter.timeUntilSpawn = glm::get_random_between(particleEmitter.spawnRange);
  }
  
  for (int pidx = 0; pidx < particleEmitter.particles.size(); pidx++) {
    Particle& particle = particleEmitter.particles[pidx];
    if (!particle.alive) {
      continue;
    }
    
    particle.timeAlive += deltaTime;
    if (particle.timeAlive > particleEmitter.particleLife) {
      particle.alive = false;
      particle.timeAlive = 0;
      particleEmitter.inactiveParticles.push(pidx);
    }
    
    particle.position = particle.position + particle.velocity * deltaTime;
    particle.velocity = particleEmitter.initialVelocity * particleEmitter.velocityFunction(particle.timeAlive / particleEmitter.particleLife);
  }
}

void render_particle_emitter(const ParticleEmitter& particleEmitter, const Shader& shader, const Camera& camera) {
  camera.render(shader);
  shader.set_uniform_matrix_4fv("uModel", 1, GL_FALSE, glm::value_ptr(particleEmitter.model));

  glBindVertexArray(particleEmitter.vao);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  for (const Particle& particle : particleEmitter.particles) {
    if (!particle.alive) continue;
    
    shader.set_uniform_1f("uTransparency", 1 - particle.timeAlive / particleEmitter.particleLife);
    shader.set_uniform_3f("uPosition", particle.position.x, particle.position.y, particle.position.z);
    glDrawElements(GL_TRIANGLE_FAN, particleEmitter.numVertices, GL_UNSIGNED_INT, 0);
  }
 
  glBindVertexArray(0);

  glDisable(GL_BLEND);
}
