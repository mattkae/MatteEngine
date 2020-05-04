#pragma once
#include "Matrix4x4f.h"
#include "Vector3f.h"
#include "Quaternion.h"

struct Bone {
	Matrix4x4f offsetMatrix;
	Matrix4x4f localTransform;
};

struct BoneTreeNode {
    int boneIndex = -1;
    BoneTreeNode* children;
    unsigned int numChildren = 0;
    Matrix4x4f nodeTransform;

    void update(Bone* bones, Matrix4x4f* matrices, Matrix4x4f currentMatrix, Matrix4x4f inverseRootNode);
    void free();
};

