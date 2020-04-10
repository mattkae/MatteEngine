#pragma once
#include "Matrix4x4f.h"
#include "Vector3f.h"
#include "Quaternion.h"

struct Bone {
	Matrix4x4f offsetMatrix;
	Vector3f position;
	Vector3f scaling;
	Quaternion rotation;

	int parentBoneIndex = -1;
	unsigned int numChildBones;
	int* childrenBoneIndices = nullptr;

	void free();
};