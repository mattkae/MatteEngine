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