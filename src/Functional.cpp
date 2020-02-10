#define _USE_MATH_DEFINES

#include "Functional.h"
#include "MathHelper.h"

constexpr float getTwoPi() {
	return static_cast<float>(2 * PI);
}

float calculateFunc(const FunctionFloat& f, float fractionComplete) {
    return f.initial 
		+ (f.linear * fractionComplete) 
		+ (f.quadratic * fractionComplete * fractionComplete);
}

Vector3f calculateFunc(const FunctionVec3& f, float fractionComplete) {
    return f.initial 
		+ (f.linear * fractionComplete) 
		+ (f.quadratic * fractionComplete * fractionComplete)
		+ getVec3(cos(fractionComplete * getTwoPi())) * f.cosValue
		+ getVec3(sin(fractionComplete * getTwoPi())) * f.sinValue;
}

Vector4f calculateFunc(const FunctionVec4& f, float fractionComplete) {
    return f.initial 
		+ (f.linear * fractionComplete) 
		+ (f.quadratic * fractionComplete * fractionComplete)
		+ getVec4(cos(fractionComplete * getTwoPi())) * f.cosValue
		+ getVec4(sin(fractionComplete * getTwoPi())) * f.sinValue;
}