#ifndef MODEL_H
#define MODEL_H
#include "Mesh.h"
#include <vector>

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
class Shader;

class Model {
 public:
  Model(const char* path);
  void render(Shader * shader);
 private:
  void process_node(aiNode* node, const aiScene* scene);
  Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
  void process_material(aiMaterial* mat);

  std::vector<Mesh> mMeshes;
};

#endif
