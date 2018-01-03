#include "Material.h"

// Define default values
const glm::vec3 DEFAULT_EMISSIVE = glm::vec3(1.0, 1.0, 1.0);
const glm::vec3 DEFAULT_DIFFUSE = glm::vec3(1.0, 1.0, 1.0);
const glm::vec3 DEFAULT_SPECULAR  = glm::vec3(1.0, 1.0, 1.0);

Material::Material() {
  mEmissive = DEFAULT_EMISSIVE;
  mDiffuse = DEFAULT_DIFFUSE;
  mSpecular = DEFAULT_SPECULAR;
}
