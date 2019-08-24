#ifndef SPHERE_H
#define SPHERE_H

#include "Mesh.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

class Shader;

struct Sphere {
    int radius = 1;
	float angleIncrements = 5.f;
    glm::mat4 model = glm::mat4(1.0);
    Mesh mesh;
};

void initializeSphere(Sphere& sphere);
void renderSphere(const Sphere& sphere, const Shader& shader, bool withMaterial = true);
void freeSphere(Sphere& sphere);

void to_json(nlohmann::json &j, const Sphere &sphere);
void from_json(const nlohmann::json &j, Sphere &sphere);

#endif
