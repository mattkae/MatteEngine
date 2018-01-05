#include "gl_includes.h"
#include "Model.h"
#include "Logger.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <string>

Model::Model(const char* path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    logger::log_error(importer.GetErrorString());
    return;
  }
  
  //this->directory = path.substr(0, path.find_last_of('/'));
  process_node(scene->mRootNode, scene);
}

void Model::render(Shader* shader) {
  for (int meshIndex = 0; meshIndex < mMeshes.size(); meshIndex++) {
    mMeshes[meshIndex].render(shader);
  }
}

void Model::process_node(aiNode* node, const aiScene* scene) {
  for (int meshIndex = 0; meshIndex < node->mNumMeshes; meshIndex++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[meshIndex]];

    mMeshes.push_back(process_mesh(mesh, scene));
  }

  for (int childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
    process_node(node->mChildren[childIndex], scene);
  }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
  Mesh result;
  
  // Vertices
    for (int vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
    Vertex vertex;
    glm::vec3 position = glm::vec3(mesh->mVertices[vertexIndex].x,
			  mesh->mVertices[vertexIndex].y,
			  mesh->mVertices[vertexIndex].z);

    glm::vec3 normal = glm::vec3(mesh->mNormals[vertexIndex].x,
			mesh->mNormals[vertexIndex].y,
			mesh->mNormals[vertexIndex].z);

    glm::vec2 texCoords;
    if (mesh->HasTextureCoords(vertexIndex)) {
      texCoords = glm::vec2(mesh->mTextureCoords[0][vertexIndex].x,
			    mesh->mTextureCoords[0][vertexIndex].y);
    }

    vertex.position = position;
    vertex.normal = normal;
    vertex.texCoord = texCoords;
    result.add_vertex(vertex);
  }

  // Indices
  for (int faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
    aiFace face = mesh->mFaces[faceIndex];
    
    for (int indexIndex = 0; indexIndex < face.mNumIndices; indexIndex++) {
      unsigned int index = face.mIndices[indexIndex];
      result.add_index(index);
    }
  }

  // Generate vao/vbos and bind
  result.generate();
  return result;
}

void Model::process_material(aiMaterial* mat) {
  // TO-DO
}
