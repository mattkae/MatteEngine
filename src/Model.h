#ifndef MODEL_H
#define MODEL_H
#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <glm/glm.hpp>

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
class Shader;

class Model {
 public:
  Model(const char* path);
  void render(Shader * shader);
  void set_model(glm::mat4 model) { mModel = model; };
 private:
  void process_node(aiNode* node, const aiScene* scene);
  Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
  Material process_material(aiMaterial* mat);

  std::vector<Mesh> mMeshes;
  glm::mat4 mModel;
};

#endif
