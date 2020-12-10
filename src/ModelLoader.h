#pragma once
#include "Model.h"
#include "Box.h"
#include "List.h"
#include "GeneratedTexture.h"
#include <vector>

struct LoadModel;
struct ModelLoadResult {
	Model model;
	Box3D box;
};

struct ModelLoader {
	List<GeneratedTexture> textureList;
	void loadTextureList(const char* path);
	ModelLoadResult loadSerializedModel(const char* path);
	ModelLoadResult loadFromLoadModel(LoadModel& intermediateModel);
	void free();
};