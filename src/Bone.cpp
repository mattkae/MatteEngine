#include "Bone.h"

void BoneTreeNode::update(Bone* bones, Matrix4x4f* matrices, Matrix4x4f currentMatrix, Matrix4x4f inverseRootNode) {
	if (boneIndex > -1) {
		currentMatrix = bones[boneIndex].localTransform * currentMatrix;
		matrices[boneIndex] = bones[boneIndex].offsetMatrix * currentMatrix * inverseRootNode;
	} else {
		currentMatrix = nodeTransform * currentMatrix;
	}

	for (unsigned int childIdx = 0; childIdx < numChildren; childIdx++) {
		children[childIdx].update(bones, matrices, currentMatrix, inverseRootNode);
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