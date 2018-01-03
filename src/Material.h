#ifndef MATERIAL_H
#define MATERIAL_H
#include "gl_includes.h"
#include <glm/glm.hpp>  

class Material {
 public:
  Material();
  Material(glm::vec3 emissive, glm::vec3 diffuse, glm::vec3 specular);
  Material(glm::vec3 emissive, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess);
  void render();

 private:
  glm::vec3 mEmissive;
  glm::vec3 mDiffuse;
  glm::vec3 mSpecular;
  GLfloat mShininess = 0;
};

#endif
