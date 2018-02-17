#include "gl_includes.h"
#include "Model.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

Model::Model(const char* path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    cerr << "Unable to load scene from path " << path << " because: " << importer.GetErrorString() << endl;
    return;
  }

  mDirectory = path;
  std::size_t found = mDirectory.find_last_of("/");
  mDirectory = mDirectory.substr(0, found);

  process_node(scene->mRootNode, scene);

  mModel = glm::mat4(1.0);
}

void Model::render(Shader* shader) {
  shader->SetUniformMatrix4fv("uModel", 1, GL_FALSE, glm::value_ptr(mModel));
  for (auto mesh : mMeshes) {
    mesh.render(shader);
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

  if (mesh == nullptr) {
    return result;
  }
  
  // Vertices
  for (int vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
    Vertex vertex;
    glm::vec3 position = glm::vec3(mesh->mVertices[vertexIndex].x,
			  mesh->mVertices[vertexIndex].y,
			  mesh->mVertices[vertexIndex].z);

    glm::vec3 normal;
    if (mesh->HasNormals()) {
      normal = glm::vec3(mesh->mNormals[vertexIndex].x,
			mesh->mNormals[vertexIndex].y,
			mesh->mNormals[vertexIndex].z);
    }
    
    glm::vec2 texCoords = glm::vec2(0.f);
    if (mesh->HasTextureCoords(0)) {
      texCoords = glm::vec2(mesh->mTextureCoords[0][vertexIndex].x,
			    mesh->mTextureCoords[0][vertexIndex].y);
    }

    vertex.position = position;
    vertex.normal = normal;
    vertex.texCoords = texCoords;
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
  
  // Material
  result.set_material(process_material(scene->mMaterials[mesh->mMaterialIndex]));
    
  // Generate vao/vbos and bind
  result.generate();
  return result;
}

Material Model::process_material(aiMaterial* mat) {
  Material result;

  aiColor4D diffuse, specular, emissive;
  ai_real shininess;

  if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) == AI_SUCCESS) {
    result.diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
  }
    
  if (mat->Get(AI_MATKEY_COLOR_SPECULAR, specular) == AI_SUCCESS) {
    result.specular = glm::vec4(specular.r, specular.g, specular.b, specular.a);
  }
 
  if (mat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive) == AI_SUCCESS) {
    result.emissive = glm::vec4(emissive.r, emissive.g, emissive.b, emissive.a);
  }

  for (int tindx = 0; tindx < mat->GetTextureCount(aiTextureType_DIFFUSE); tindx++) {
    aiString str;
    mat->GetTexture(aiTextureType_DIFFUSE, tindx, &str);

    if (loadedTextures.find(str.C_Str()) != loadedTextures.end()) {
      result.textures.push_back(loadedTextures[str.C_Str()]);
    } else {
      string path = mDirectory + "/" + str.C_Str();
      Texture texture = load_texture(path.c_str());
      texture.type = TEX_DIFFUSE;
      result.textures.push_back(texture);
      loadedTextures[str.C_Str()] = texture;
    }
  }

  for (int tindx = 0; tindx < mat->GetTextureCount(aiTextureType_SPECULAR); tindx++) {
    aiString str;
    mat->GetTexture(aiTextureType_SPECULAR, tindx, &str);

    if (loadedTextures.find(str.C_Str()) != loadedTextures.end()) {
      result.textures.push_back(loadedTextures[str.C_Str()]);
    } else {
      string path = mDirectory + "/" + str.C_Str();
      Texture texture = load_texture(path.c_str());
      texture.type = TEX_SPECULAR;
      result.textures.push_back(texture);
      loadedTextures[str.C_Str()] = texture;
    }
  }
    
  return result;
}
