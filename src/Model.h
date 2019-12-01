#ifndef MODEL_H
#define MODEL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "UI/UIContext.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include "Primitives/Box.h"
#include "Matrix4x4f.h"

struct Model {
    Matrix4x4f model;
    std::vector<Mesh> meshes;
};

void initializeModel(Model& model, Box& box);
void renderModel(const Model& model, const Shader& shader, bool withMaterial = true);
void freeModel(Model& model);

#endif
