#ifndef SPHERE_H
#define SPHERE_H

#include "Mesh.h"
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader;

struct Sphere {
  public:
    int radius = 1;
    glm::mat4 model = glm::mat4(1.0);
    void generate(float angleIncrements);
    void render(const Shader &shader, bool withMaterial = true) const;
    void free();

  private:
    Mesh mMesh;
};

#endif
