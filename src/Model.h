#ifndef MODEL_H
#define MODEL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <glm/glm.hpp>
#include <string>

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
class Shader;

class Model {
 public:
	Model();
	Model(std::string path, glm::mat4 transform = glm::mat4(1.0));
	void render(const Shader& shader);
	void set_model(glm::mat4 model) { mModel = model; };
	glm::mat4 get_model() const { return mModel; };
	std::string get_path() const { return mPath; };
 private:
	void process_node(aiNode* node, const aiScene* scene);
	Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
	Material process_material(aiMaterial* mat);

	//  Mesh mMesh;
	std::string mPath;
	std::vector<Mesh> mMeshes;
	glm::mat4 mModel;
	std::string mDirectory;
};

#endif
