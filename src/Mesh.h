#ifndef MESH_H
#define MESH_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Material.h"
#include "gl_includes.h"
#include <glm/glm.hpp>
#include <vector>

class Shader;

struct Vertex {
    glm::vec3 position = glm::vec3(0.0);
    glm::vec3 normal = glm::vec3(0.0);
    glm::vec2 texCoords = glm::vec2(0.0);
};

class Mesh {
  public:
    Mesh();
    void generate();
    void free_resources();
    void render(Shader &shader, bool withMaterial = true);
    void add_vertex(Vertex vertex);
    void add_index(GLint index);
    void set_material(Material material);

  private:
    bool mHasGenerated;
    GLuint mVao, mVbo, mEbo;
    std::vector<Vertex> mVertices;
    std::vector<GLint> mIndices;
    Material mMaterial;
    GLfloat mShininess;
};

#endif
