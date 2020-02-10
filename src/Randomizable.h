#pragma once
#include "Vector3f.h"
#include "Vector4f.h"

struct RandomizableFloat {
	float baseValue = 0;
	float randomOffset = 0;
};

struct RandomizableVec3 {
	Vector3f baseValue;
	Vector3f randomOffset;
};

struct RandomizableVec4 {
	Vector4f baseValue;
	Vector4f randomOffset;
};

float getRandom(const RandomizableFloat& randomizable);
Vector3f getRandom(const RandomizableVec3& randomizable);
Vector4f getRandom(const RandomizableVec4& randomizable);