#include "SceneReader.h"

#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "Scene.h"
#include "Model.h"
#include "Light.h"

using json = nlohmann::json;

/* JSON Helpers */
glm::mat4 arrayToMat4(const std::vector<float>& transformArray) {
	glm::mat4 matrix(1.0);
	for (size_t r = 0; r < 4; r++) {
		for (size_t c = 0; c < 4; c++) {
			matrix[r][c] = transformArray[r * 4 + c];
		}
	}
	return matrix;
}

std::vector<float> mat4ToArray(const glm::mat4& matrix) {
	std::vector<float> matrixArray;
	for (size_t r = 0; r < 4; r++) {
		for (size_t c = 0; c < 4; c++) {
			matrixArray.push_back(matrix[r][c]);
		}
	}
	return matrixArray;
}

glm::vec3 arrayToVec3(const std::vector<float>& vecArray) {
	glm::vec3 result(1.0);
	for (size_t v = 0; v < 3; v++) {
		result[v] = vecArray[v];
	}
	return result;
}

std::vector<float> vec3ToArray(const glm::vec3 vec) {
	std::vector<float> vecArray;
	for (size_t v = 0; v < 3; v++) {
		vecArray.push_back(vec[v]);
	}
	return vecArray;
}

// Model
void to_json(json& j, const Model& model) {
	j = json{
		{"path", model.get_path()},
		{"transform", mat4ToArray(model.get_model())}
	};
}

void from_json(const json& j, Model& model) {
	std::string path;
	std::vector<float> matrixArray;

	j.at("path").get_to(path);
	j.at("transform").get_to<std::vector<float>>(matrixArray);

	model = Model(path, arrayToMat4(matrixArray));
}

// Light
void to_json(json& j, const Light& light) {
	j = json{
		{"type", light.type},
		{"color", vec3ToArray(light.color)},
		{"direction", vec3ToArray(light.direction)},
		{"up", vec3ToArray(light.up)},
		{"position", vec3ToArray(light.position)},
		{"constant", light.constant},
		{"linear", light.linear},
		{"quadratic", light.quadratic},
		{"cosineCutOff", light.cosineCutOff},
		{"dropOff", light.dropOff},
		{"usesShadows", light.usesShadows},
		{"isOn", light.isOn}
	};
}

void from_json(const json& j, Light& light) {
	std::vector<float> color;
	std::vector<float> direction;
	std::vector<float> up;
	std::vector<float> position;

	j.at("type").get_to(light.type);
	switch (light.type) {
	case LightType::Directional:
		light = get_directional(1600, 800);
		break;
	case LightType::Point:
		light = get_point(1600, 800);
		break;
	case LightType::Spot:
		light = get_spot(1600, 800);
		break;
	}

	j.at("color").get_to<std::vector<float>>(color);
	j.at("constant").get_to(light.constant);
	j.at("linear").get_to(light.linear);
	j.at("quadratic").get_to(light.quadratic);
	j.at("cosineCutOff").get_to(light.cosineCutOff);
	j.at("dropOff").get_to(light.dropOff);
	j.at("usesShadows").get_to(light.usesShadows);
	j.at("isOn").get_to(light.isOn);

	light.color = arrayToVec3(color);

	if (light.type != LightType::Point) {
		j.at("direction").get_to<std::vector<float>>(direction);
		light.direction = arrayToVec3(direction);
	}

	if (light.type != LightType::Directional) {
		j.at("up").get_to<std::vector<float>>(up);
		j.at("position").get_to<std::vector<float>>(position);
		light.position = arrayToVec3(position);
		light.up = arrayToVec3(up);
	}
}

// Scene
void to_json(json& j, const Scene& scene) {
	j = json{
		{"models", scene.get_models()},
		{"lights", scene.get_lights()}
	};
}

void from_json(const json& j, Scene& scene) {
	std::vector<Model> models;
	std::vector<Light> lights;
	j.at("models").get_to<std::vector<Model>>(models);
	j.at("lights").get_to<std::vector<Light>>(lights);
	scene.set_models(models);
	scene.set_lights(lights);
}

Scene SceneReader::load_scene(std::string sceneFilePath) {
	std::ifstream sceneFile(sceneFilePath);
	json sceneJson = json::parse(sceneFile);
	Scene scene = sceneJson;
	sceneFile.close();
	return scene;
}