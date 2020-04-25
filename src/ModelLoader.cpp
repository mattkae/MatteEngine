#include "ModelLoader.h"
#include "BinarySerializer.h"
#include "LoadModel.h"
#include "BinarySerializer.h"
#include "TextureInfo.h"
#include "Logger.h"
#include "Mesh.h"
#include "Box.h"
#include "Bone.h"
#include <SOIL.h>

GLuint loadTexture(std::string path);

void ModelLoader::loadTextureList(const char* path) {
	textureList.allocate(16, sizeof(TextureListItem));

	BinarySerializer serializer(path, SerializationMode::READ);
	int numTextures = serializer.readInt32();
	for (int textureIndex = 0; textureIndex < numTextures; textureIndex++) {
		TextureInfo info;
		info.read(serializer);

		TextureListItem item;
		item.uniqueId = info.uniqueId;
		item.texture = loadTexture(info.fullpath);
		textureList.add(&item);
	}

	serializer.close();
}

GLuint loadTexture(std::string path) {
	GLuint texture;
	int width, height;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	const char* cStrPath = path.c_str();
    unsigned char* image = SOIL_load_image(cStrPath, &width, &height, 0, SOIL_LOAD_RGBA);
	if (image == NULL) {
		Logger::logError("Unable to load image from path: " + path);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(image);
	return texture;
}

ModelLoader::ModelLoadResult ModelLoader::loadSerializedModel(const char* path) {
	BinarySerializer serializer(path, SerializationMode::READ);
	LoadModel intermediateModel;
	intermediateModel.readLoadModel(serializer);
	serializer.close();
	return ModelLoader::loadFromLoadModel(intermediateModel);
}

void copyBoneTreeNode(BoneTreeNode* node, const LoadBoneNode& nodeToCopy) {
	node->boneIndex = nodeToCopy.boneIndex;
	node->nodeTransform = nodeToCopy.nodeTransform;
	node->numChildren = nodeToCopy.children.size();
	node->children = new BoneTreeNode[nodeToCopy.children.size()];
	for (unsigned int nodeIdx = 0; nodeIdx < node->numChildren; nodeIdx++) {
		copyBoneTreeNode(&node->children[nodeIdx], nodeToCopy.children[nodeIdx]);
	}
}

ModelLoader::ModelLoadResult ModelLoader::loadFromLoadModel(LoadModel& intermediateModel) {
	ModelLoader::ModelLoadResult retval;
	retval.model.inverseRootNode = intermediateModel.inverseRootNode;
	retval.model.numMeshes = intermediateModel.meshes.size();
	retval.model.meshes = new Mesh[retval.model.numMeshes];
	retval.model.numBones = intermediateModel.bones.size();
	retval.model.bones = new Bone[retval.model.numBones];
	for (unsigned int boneIdx = 0; boneIdx < retval.model.numBones; boneIdx++) {
		retval.model.bones[boneIdx].offsetMatrix = intermediateModel.bones[boneIdx].offsetMatrix;
	}

	if (intermediateModel.rootNode.children.size() > 0) {
		retval.model.rootNode = new BoneTreeNode();
		copyBoneTreeNode(retval.model.rootNode, intermediateModel.rootNode);
	}

	for (int meshIdx = 0; meshIdx < retval.model.numMeshes; meshIdx++) {
		retval.model.meshes[meshIdx].initialize(intermediateModel.meshes[meshIdx], &textureList);
	}

	retval.box.lowerLeft = intermediateModel.lowerLeftBoundingBoxCorner;
	retval.box.upperRight = intermediateModel.upperRightBoundingBoxCorner;
	retval.model.animationController.numAnimations = intermediateModel.animations.size();
	retval.model.animationController.animationList  = new Animation[retval.model.animationController.numAnimations];
	for (unsigned int animationIdx = 0; animationIdx < retval.model.animationController.numAnimations; animationIdx++) {
		retval.model.animationController.animationList[animationIdx] = intermediateModel.animations[animationIdx];
	}

	return retval;
}

void ModelLoader::free() {
	// @TODO: Remove all textures 
}