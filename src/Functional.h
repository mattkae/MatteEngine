#pragma once
#include "GlmUtility.h"
#include <glm/glm.hpp>

struct FunctionFloat {
	float initial = 0.f;
	float linear = 0.f;
	float quadratic = 0.f;
	float sinValue = 0.f;
	float cosValue = 0.f;
};

struct FunctionVec3 {
	glm::vec3 initial = glm::vec3(0);
	glm::vec3 linear = glm::vec3(0);
	glm::vec3 quadratic = glm::vec3(0);
	glm::vec3 sinValue = glm::vec3(0);;
	glm::vec3 cosValue = glm::vec3(0);;
};

struct FunctionVec4 {
	glm::vec4 initial = glm::vec4(0);
	glm::vec4 linear = glm::vec4(0);
	glm::vec4 quadratic = glm::vec4(0);
	glm::vec4 sinValue = glm::vec4(0);
	glm::vec4 cosValue = glm::vec4(0);
};

float calculateFunc(const FunctionFloat& f, float fractionComplete);
glm::vec3 calculateFunc(const FunctionVec3& f, float fractionComplete);
glm::vec4 calculateFunc(const FunctionVec4& f, float fractionComplete);