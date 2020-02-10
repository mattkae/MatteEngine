#pragma once
#include "Vector3f.h"
#include "Vector4f.h"

struct FunctionFloat {
	float initial = 0.f;
	float linear = 0.f;
	float quadratic = 0.f;
	float sinValue = 0.f;
	float cosValue = 0.f;
};

struct FunctionVec3 {
	Vector3f initial;
	Vector3f linear;
	Vector3f quadratic;
	Vector3f sinValue;
	Vector3f cosValue;
};

struct FunctionVec4 {
	Vector4f initial;
	Vector4f linear;
	Vector4f quadratic;
	Vector4f sinValue;
	Vector4f cosValue;
};

float calculateFunc(const FunctionFloat& f, float fractionComplete);
Vector3f calculateFunc(const FunctionVec3& f, float fractionComplete);
Vector4f calculateFunc(const FunctionVec4& f, float fractionComplete);