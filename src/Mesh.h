#ifndef MESH_H
#define MESH_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Material.h"
#include "Vertex.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;
class Mesh {
  public:
    Mesh();
    void generate();
    void free_resources();
    void render(const Shader &shader, bool withMaterial = true) const;
    void set_material(Material material);
    std::vector<Vertex> mVertices;
    std::vector<GLint> mIndices;

  private:
    bool mHasGenerated;
    GLuint mVao, mVbo, mEbo;
    Material mMaterial;
    GLfloat mShininess;
};

#endif
