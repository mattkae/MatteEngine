#include "Model.h"
#include "GlmUtility.h"
#include "Shader.h"
#include "ObjModel.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

void Model::free()
{
    for (auto mesh : mMeshes) {
        mesh.free_resources();
    }

    mMeshes.clear();
}

void Model::render(const Shader& shader, bool withMaterial) const
{
    shader.set_uniform_matrix_4fv("uModel", 1, GL_FALSE,
        glm::value_ptr(model));
    for (auto mesh : mMeshes) {
        mesh.render(shader, withMaterial);
    }
}

void Model::generate()
{
    for (auto& mesh : mMeshes) {
        mesh.generate();
    }
}

void to_json(nlohmann::json& j, const Model& model)
{
    j = nlohmann::json { { "transform", glm::mat4ToArray(model.model) } };
}

void from_json(const nlohmann::json& j, Model& model)
{
    std::string path;
    std::vector<float> matrixArray;

    j.at("path").get_to(path);
	model = loadFromObj(path);

    j.at("transform").get_to<std::vector<float>>(matrixArray);
    model.model = glm::arrayToMat4(matrixArray);
}