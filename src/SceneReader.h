#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "Scene.h"
#include "Model.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void to_json(json& j, const Model& model) {
	glm::mat4 transform = model.get_model();
	float transformArray[16];
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			transformArray[r * 4 + c] = transform[r][c];
		}
	}
	j = json{
		{"path", model.get_path()},
		{"transform", transformArray}
	};
}

void from_json(const json& j, Model& model) {
	std::string path;
	std::vector<float> transformArray;
	j.at("path").get_to(path);
	j.at("transform").get_to<std::vector<float>>(transformArray);

	glm::mat4 transform;
	for (int r = 0; r < 4; r++) {
		for (int c = 0; c < 4; c++) {
			transform[r][c] = transformArray[r * 4 + c];
		}
	}

	model = Model(path, transform);
}

void to_json(json& j, const Scene& scene) {
	j = json{ 
		{"models", scene.get_models()}, 
	};
}

void from_json(const json& j, Scene& scene) {
	std::vector<Model> models;
	j.at("models").get_to<std::vector<Model>>(models);
	scene.set_models(models);
}

namespace SceneReader {
	Scene load_scene(std::string sceneFilePath) {
		std::ifstream sceneFile(sceneFilePath);
		json sceneJson = json::parse(sceneFile);
		Scene scene = sceneJson;
		sceneFile.close();
		return scene;
	}
}