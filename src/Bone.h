#pragma once
#include "Matrix4x4f.h"

struct Bone {
	Matrix4x4f transform;
	Matrix4x4f offsetMatrix;
	unsigned int nodeUniqueId;
	unsigned int parentNodeUniqueId;
};