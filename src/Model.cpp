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

void to_json(nlohmann::json& j, const Model& model) {
    j = nlohmann::json { { "transform", glm::mat4ToArray(model.model) } };
}

void from_json(const nlohmann::json& j, Model& model) {
    std::string path;
    std::vector<float> matrixArray;

    j.at("path").get_to(path);
	model = loadFromObj(path);

    j.at("transform").get_to<std::vector<float>>(matrixArray);
    model.model = glm::arrayToMat4(matrixArray);
}