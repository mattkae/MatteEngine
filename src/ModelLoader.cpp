#include "ModelLoader.h"
#include "BinarySerializer.h"
#include "LoadModel.h"
#include "BinarySerializer.h"
#include "TextureInfo.h"
#include "Logger.h"
#include "Mesh.h"
#include "Box.h"
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

ModelLoader::ModelLoadResult ModelLoader::loadFromLoadModel(LoadModel& intermediateModel) {
	ModelLoader::ModelLoadResult retval;
	retval.model.numMeshes = intermediateModel.meshes.size();
	retval.model.meshes = new Mesh[retval.model.numMeshes];
	for (int meshIdx = 0; meshIdx < retval.model.numMeshes; meshIdx++) {
		retval.model.meshes[meshIdx].initialize(intermediateModel.meshes[meshIdx], &textureList);
	}

	retval.box.lowerLeft = intermediateModel.lowerLeftBoundingBoxCorner;
	retval.box.upperRight = intermediateModel.upperRightBoundingBoxCorner;

	return retval;
}

void ModelLoader::free() {
	// @TODO: Remove all textures 
}