#ifndef MODEL_H
#define MODEL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "UIContext.h"
#include <string>
#include <vector>
#include "Box.h"
#include "Matrix4x4f.h"

struct Model {
    Matrix4x4f model;
    Mesh* meshes;
    int numMeshes;

    void render(const Shader& shader, bool withMaterial = true) const;
    void free();
};

#endif
