#include "Material.h"

// Define default values
const glm::vec3 DEFAULT_EMISSIVE = glm::vec3(1.0, 1.0, 1.0);
const glm::vec3 DEFAULT_DIFFUSE = glm::vec3(1.0, 1.0, 1.0);
const glm::vec3 DEFAULT_SPECULAR  = glm::vec3(1.0, 1.0, 1.0);
const GLfloat DEFAULT_SHININESS = 2.7;

Material::Material() {
  mEmissive = DEFAULT_EMISSIVE;
  mDiffuse = DEFAULT_DIFFUSE;
  mSpecular = DEFAULT_SPECULAR;
  mShininess = DEFAULT_SHININESS;
}

Material::Material(glm::vec3 emissive, glm::vec3 diffuse, glm::vec3 specular) {
  mEmissive = emissive;
  mDiffuse = diffuse;
  mSpecular = specular;
  mShininess = DEFAULT_SHININESS;
}

Material::Material(glm::vec3 emissive, glm::vec3 diffuse, glm::vec3 specular, GLfloat shininess) {
  mEmissive = emissive;
  mDiffuse = diffuse;
  mSpecular = specular;
  mShininess = shininess;
}

void Material::render() {
  // TO-DO: Implement render
}
