#ifndef MESH_H
#define MESH_H
#include "gl_includes.h"
#include "Material.h"
#include <glm/glm.hpp>
#include <vector>

class Shader;

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoord;
};

class Mesh {
 public:
   Mesh();
  ~Mesh();
  void generate();
  void render(Shader* shader);
  void add_vertex(Vertex vertex);
  void add_index(GLint index);
  void add_material(Material material);
 private:
  GLuint mVao, mVbo, mEbo;
  std::vector<Vertex> mVertices;
  std::vector<GLint>  mIndices;
  std::vector<Material> mMaterials;
  GLfloat mShininess;
};

#endif
