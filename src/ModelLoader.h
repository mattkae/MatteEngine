#pragma once
#include "Model.h"
#include "Box.h"
#include "List.h"
#include "TextureListItem.h"
#include <vector>

struct LoadModel;

struct ModelLoader {
	struct ModelLoadResult {
		Model model;
		Box box;
	};

	List<TextureListItem> textureList;
	void loadTextureList(const char* path);
	ModelLoadResult loadSerializedModel(const char* path);
	ModelLoadResult loadFromLoadModel(LoadModel& intermediateModel);
	void free();
};