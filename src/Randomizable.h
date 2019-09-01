#pragma once
#include "GlmUtility.h"
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

struct RandomizableFloat {
	float baseValue = 0;
	float randomOffset = 0;
};

struct RandomizableVec3 {
	glm::vec3 baseValue = glm::vec3(0);
	glm::vec3 randomOffset = glm::vec3(0);
};

struct RandomizableVec4 {
	glm::vec4 baseValue = glm::vec4(0);
	glm::vec4 randomOffset = glm::vec4(0);
};

float getRandom(const RandomizableFloat& randomizable);
glm::vec3 getRandom(const RandomizableVec3& randomizable);
glm::vec4 getRandom(const RandomizableVec4& randomizable);

void to_json(nlohmann::json& j, const RandomizableFloat& r);
void from_json(const nlohmann::json& j, RandomizableFloat& r);
void to_json(nlohmann::json& j, const RandomizableVec3& r);
void from_json(const nlohmann::json& j, RandomizableVec3& r);
void to_json(nlohmann::json& j, const RandomizableVec4& r);
void from_json(const nlohmann::json& j, RandomizableVec4& r);