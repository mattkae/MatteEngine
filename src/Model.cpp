#include "Model.h"
#include "GlmUtility.h"
#include "ObjModel.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void freeModel(Model& model) {
    for (auto& mesh : model.meshes) {
        freeMesh(mesh);
    }

    model.meshes.clear();
}

void renderModel(const Model& model, const Shader& shader, bool withMaterial) {
    setShaderMat4(shader, "uModel", model.model);
    for (auto& mesh : model.meshes) {
		renderMesh(mesh, shader, withMaterial);
    }
}

void initializeModel(Model& model) {
    for (auto& mesh : model.meshes) {
        initializeMesh(mesh);
    }
}