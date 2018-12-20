#ifndef MATERIAL_H
#define MATERIAL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "gl_includes.h"
#include "Texture.h"
#include <glm/glm.hpp>
#include <vector>

class Shader;

struct Material {
  glm::vec4 emissive = glm::vec4(1.0);
  glm::vec4 diffuse = glm::vec4(1.0);
  glm::vec4 specular = glm::vec4(1.0);
  glm::vec3 diffuseProperty = glm::vec3(1.0);
  glm::vec3 specularProperty = glm::vec3(1.0);
  GLfloat shininess = 20.f;
  std::vector<Texture> textures;
};

void render_material(const Material& material, Shader& shader);

#endif
