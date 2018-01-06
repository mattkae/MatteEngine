#include "Material.h"

// Define default values
const glm::vec4 DEFAULT_EMISSIVE = glm::vec4(1.0, 1.0, 1.0, 0.0);
const glm::vec4 DEFAULT_DIFFUSE = glm::vec4(1.0, 1.0, 1.0, 0.0);
const glm::vec4 DEFAULT_SPECULAR  = glm::vec4(1.0, 1.0, 1.0, 0.0);
const glm::vec4 DEFAULT_AMBIENT  = glm::vec4(1.0, 1.0, 1.0, 0.0);
const GLfloat DEFAULT_SHININESS = 2.7;

Material::Material() {
  mEmissive = DEFAULT_EMISSIVE;
  mDiffuse = DEFAULT_DIFFUSE;
  mAmbient = DEFAULT_AMBIENT;
  mSpecular = DEFAULT_SPECULAR;
  mShininess = DEFAULT_SHININESS;
}

Material::Material(const Material& other) {
  this->mEmissive = other.mEmissive;
  this->mDiffuse = other.mDiffuse;
  this->mSpecular = other.mSpecular;
  this->mShininess = other.mShininess;
}

Material::Material(glm::vec4 emissive, glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular) {
  mEmissive = emissive;
  mDiffuse = diffuse;
  mAmbient = ambient;
  mSpecular = specular;
  mShininess = DEFAULT_SHININESS;
}

Material::Material(glm::vec4 emissive, glm::vec4 diffuse, glm::vec4 ambient, glm::vec4 specular, GLfloat shininess) {
  mEmissive = emissive;
  mDiffuse = diffuse;
  mAmbient = ambient;
  mSpecular = specular;
  mShininess = shininess;
}

void Material::set_diffuse(float r, float g, float b, float a) {
  mDiffuse = glm::vec4(r, g, b, a);
}

void Material::set_specular(float r, float g, float b, float a) {
  mSpecular = glm::vec4(r, g, b, a);
}

void Material::set_ambient(float r, float g, float b, float a) {
  mAmbient = glm::vec4(r, g, b, a);
}

void Material::set_emissive(float r, float g, float b, float a) {
  mEmissive = glm::vec4(r, g, b, a);
}

void Material::render() {
  // TO-DO: Implement render
}
