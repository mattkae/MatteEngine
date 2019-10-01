#define _USE_MATH_DEFINES

#include "Functional.h"
#include <cmath>

constexpr float getTwoPi() {
	return static_cast<float>(2 * M_PI);
}

float calculateFunc(const FunctionFloat& f, float fractionComplete) {
    return f.initial 
		+ (f.linear * fractionComplete) 
		+ (f.quadratic * fractionComplete * fractionComplete);
}

glm::vec3 calculateFunc(const FunctionVec3& f, float fractionComplete) {
    return f.initial 
		+ (f.linear * fractionComplete) 
		+ (f.quadratic * fractionComplete * fractionComplete)
		+ f.cosValue * glm::vec3(cos(fractionComplete * getTwoPi()))
		+ f.sinValue * glm::vec3(sin(fractionComplete * getTwoPi()));
}

glm::vec4 calculateFunc(const FunctionVec4& f, float fractionComplete) {
    return f.initial 
		+ (f.linear * fractionComplete) 
		+ (f.quadratic * fractionComplete * fractionComplete)
		+ f.cosValue * glm::vec4(cos(fractionComplete * getTwoPi()))
		+ f.sinValue * glm::vec4(sin(fractionComplete * getTwoPi()));
}

void to_json(nlohmann::json& j, const FunctionVec3& config) {
}

void from_json(const nlohmann::json& j, FunctionVec3& functionConfig) {
    glm::from_json(j, "initial", functionConfig.initial);
    glm::from_json(j, "linear", functionConfig.linear);
    glm::from_json(j, "quadratic", functionConfig.quadratic);
    glm::from_json(j, "sinValue", functionConfig.sinValue);
    glm::from_json(j, "cosValue", functionConfig.cosValue);
}

void to_json(nlohmann::json& j, const FunctionVec4& config) {
}

void from_json(const nlohmann::json& j, FunctionVec4& functionConfig) {
    glm::from_json(j, "initial", functionConfig.initial);
    glm::from_json(j, "linear", functionConfig.linear);
    glm::from_json(j, "quadratic", functionConfig.quadratic);
	glm::from_json(j, "sinValue", functionConfig.sinValue);
    glm::from_json(j, "cosValue", functionConfig.cosValue);
}