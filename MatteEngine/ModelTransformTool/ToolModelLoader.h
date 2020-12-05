#pragma once
#include "List.h"
#include "LoadModel.h"
#include "TextureInfo.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h> 
#include <assimp/postprocess.h>
#include <GL/glew.h>


struct ToolModelLoader {
	Assimp::Importer importer;
	List<LoadModel> mModelList;
	List<TextureInfo> mTextureList;
	GLuint nextTextureUniqueId = 1;

	bool loadModel(char* path);
	bool loadTexture(char* path);
	void writeModels();
	void writeTextures();
	void processNode(char* fullpath, const aiNode* node, const aiScene* scene, LoadModel& model);
	void processAnimations(aiAnimation** const animations, unsigned int numAnimations,LoadModel& model);
	void postProcessBones(LoadModel& model, const aiScene* scene);
};