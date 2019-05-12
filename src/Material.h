#ifndef MATERIAL_H
#define MATERIAL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Texture.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Shader;

struct Material {
    std::string name;
    glm::vec3 emissive = glm::vec3(0.0);
    glm::vec3 ambient = glm::vec3(1.0);
    glm::vec3 diffuse = glm::vec3(1.0);
    glm::vec3 specular = glm::vec3(1.0);
    glm::vec3 diffuseProperty = glm::vec3(1.0);
    glm::vec3 specularProperty = glm::vec3(1.0);
    float specularComponent = 20.f;
    float opacity = 1.f;
    float dissolved = 1.0;
};

void render_material(const Shader &shader, const Material &material);

#endif
