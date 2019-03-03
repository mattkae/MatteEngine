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
    void load_model(std::string path, glm::mat4 transform = glm::mat4(1.0));
    void render(const Shader &shader, bool withMaterial = true) const;
    std::string get_path() const { return mPath; };
    glm::mat4 model;
    void free();

  private:
    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
    Material process_material(aiMaterial *mat);

    std::string mPath;
    std::vector<Mesh> mMeshes;
    std::string mDirectory;
};

void to_json(json &j, const Model &model);
void from_json(const json &j, Model &model);

#endif
