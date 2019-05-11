#ifndef MODEL_H
#define MODEL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Material.h"
#include "Mesh.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

using json = nlohmann::json;

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
class Shader;

class Model {
public:
    Model();
    void render(const Shader& shader, bool withMaterial = true) const;
    glm::mat4 model;
    std::vector<Mesh> mMeshes;
    void free();
};

void to_json(json& j, const Model& model);
void from_json(const json& j, Model& model);

#endif
