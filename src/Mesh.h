#ifndef MESH_H
#define MESH_H
#include "gl_includes.h"
#include "Material.h"
#include <glm/glm.hpp>
#include <vector>

struct Vertex {
  GLfloat position[3];
  GLfloat normal[3];
  GLfloat texCoord[2];
};

class Mesh {
 public:
  Mesh(std::vector<Vertex> vertices, std::vector<GLint> indices, std::vector<Material> materials);
  ~Mesh();
  void generate();

 private:
  GLuint mVao, mVbo, mEbo;
  std::vector<Vertex> mVertices;
  std::vector<GLint> mIndices;
  std::vector<Material> mMaterials;
  GLfloat mShininess;
};

#endif
