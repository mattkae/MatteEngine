#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "Scene.h"

using json = nlohmann::json;

void to_json(json& j, const ModelInfo& modelInfo) {
	float transformArray[16];
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			transformArray[r * 4 + c] = modelInfo.transform[r][c];
		}
	}
	j = json{
		{"path", modelInfo.path},
		{"transform", transformArray}
	};

}

void from_json(const json& j, ModelInfo& modelInfo) {
	std::vector<float> transformArray;
	j.at("path").get_to(modelInfo.path);
	j.at("transform").get_to<std::vector<float>>(transformArray);
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			modelInfo.transform[r][c] = transformArray[r * 4 + c];
		}
	}
}

void to_json(json& j, const SceneInfo& sceneInfo) {
	j = json{ 
		{"models", sceneInfo.models}, 
	};
}

void from_json(const json& j, SceneInfo& sceneInfo) {
	j.at("models").get_to<std::vector<ModelInfo>>(sceneInfo.models);
}

namespace SceneReader {
	SceneInfo readScene(std::string file) {
		std::ifstream sceneFile(file);
		json sceneJson = json::parse(sceneFile);
		sceneFile.close();
		SceneInfo scene = sceneJson;
		return scene;
	}
}