#include "Bone.h"

void BoneTreeNode::update(Bone* bones, Matrix4x4f* matrices, Matrix4x4f currentMatrix, Matrix4x4f inverseRootNode) {
	Matrix4x4f globalTransform = currentMatrix;
	if (boneIndex > -1) {
		globalTransform = globalTransform * bones[boneIndex].nodeTransform * bones[boneIndex].localTransform;
		matrices[boneIndex] = inverseRootNode * globalTransform * bones[boneIndex].offsetMatrix;
	}
	for (unsigned int childIdx = 0; childIdx < numChildren; childIdx++) {
		children[childIdx].update(bones, matrices, globalTransform, inverseRootNode);
	}
}

void BoneTreeNode::free() {
	for (unsigned int childIdx = 0; childIdx < numChildren; childIdx++) {
		children[childIdx].free();
	}

	if (children != nullptr) {
		delete []children;
	}

	numChildren = 0;
}