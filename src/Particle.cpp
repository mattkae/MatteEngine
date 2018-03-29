#include "Particle.h"
#include "Shader.h"
#include <vector>
#include <cmath>

void initialize_particle(Particle& particle) {
  glGenVertexArrays(1, &particle.vao);
  glGenBuffers(1, &particle.vbo);
  glGenBuffers(1, &particle.ebo);

  std::vector<glm::vec3> vertices(2 * particle.numVertices);
  float theta = (360.f / particle.numVertices);

  // Vertices
  for (int vidx = 0; vidx < particle.numVertices; vidx++) {
    GLfloat x = sin(theta * vidx) * particle.particleDimension.x;
    GLfloat y = sin(theta * vidx) * particle.particleDimension.y;
    GLfloat z = sin(theta * vidx) * particle.particleDimension.z;

    vertices.push_back(glm::vec3(x, y, z));
    vertices.push_back(particle.color);
  }

  // Indices
  std::vector<GLuint> indices(particle.numVertices);
  for (int iidx = 0; iidx < particle.numVertices; iidx++) {
    indices.push_back(iidx);
  }
  
  glBindVertexArray(particle.vao);;
  glBindBuffer(GL_ARRAY_BUFFER, particle.vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)0);
  
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 2 * sizeof(glm::vec3), (GLvoid*)(3 * sizeof(GLfloat)));

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, particle.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, particle.numVertices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
  
  glBindVertexArray(0);
}

void render_particle() {
  glEnableVertexAttribArray(0);
}
