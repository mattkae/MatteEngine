#include "Mesh.h"
#include "Logger.h"

Mesh::Mesh() {
  mVao = 0;
  mVbo = 0;
  mEbo = 0;
}

Mesh::~Mesh() {
  
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
  glBufferData(GL_ARRAY_BUFFER, this->mVertices.size() * sizeof(Vertex), &this->mVertices[0], GL_STATIC_DRAW);

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

void Mesh::render(Shader* shader) {
  
  glBindVertexArray(this->mVao);
  glDrawElements(GL_TRIANGLES, this->mIndices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Mesh::add_vertex(Vertex vertex) {
  this->mVertices.push_back(vertex);
}

void Mesh::add_index(GLint index) {
  this->mIndices.push_back(index);
}

void Mesh::add_material(Material material) {
  this->mMaterials.push_back(material);
}