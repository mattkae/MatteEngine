#ifndef SPHERE_H
#define SPHERE_H

#include "Model.h"
#include "Shader.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

struct Sphere {
    int radius = 1;
	float angleIncrements = 5.f;
    Matrix4x4f model;
};

void initializeSphere(const Sphere& sphere, Model& model, Box& box);

#endif
