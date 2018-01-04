#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLint> indices, std::vector<Material> materials) {
  mVertices = vertices;
  mIndices = indices;
  mMaterials = materials;
}

void Mesh::generate() {
  // Generate our buffers
  glGenVertexArrays(1, &this->mVao);
  glGenBuffers(1, &this->mVbo);
  glGenBuffers(1, &this->mEbo);

  // Bind data to VAO
  glBindVertexArray(this->mVao);

  // Put the vertex data into OpenGL
  glBindBuffer(GL_ARRAY_BUFFER, this->mVbo);
  glBufferData(GL_ARRAY_BUFFER, this->mVertices.size() * sizeof(Vertex), & this->mVertices[0], GL_STATIC_DRAW);

  // Put the index data into OpenGL
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->mIndices.size() * sizeof(GLuint), &this->mIndices[0], GL_STATIC_DRAW);

  // Position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) 0);

  // Normal
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));

  // Texture Coordinate
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) offsetof(Vertex, normal));

  // Unbind the VAO
  glBindVertexArray(0);
}
