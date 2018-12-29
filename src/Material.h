#ifndef MATERIAL_H
#define MATERIAL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Texture.h"
#include <GL/glew.h>
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
    GLfloat opacity = 1.f;
    std::vector<Texture> textures;
};

void render_material(const Shader &shader, const Material &material);

#endif
