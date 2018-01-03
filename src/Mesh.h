#ifndef MESH_H
#define MESH_H
#include "gl_includes.h"
#include "Material.h"
#include <glm/glm.hpp>
#include <vector>

class Mesh {

  
 public:
  Mesh();
  ~Mesh();

 private:
  GLuint mVao, mVbo, mEbo;
  std::vector<Vertex> mVertices;
  std::vector<GLint> mIndices;
  std::vector<Material> mMaterials;
  GLfloat shininess;
}


#endif MESH_H
