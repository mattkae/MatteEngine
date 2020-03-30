#include "AnimationController.h"
#include "Bone.h"

void AnimationController::update(float dt, Bone* bones, unsigned int numBones) {
	for (unsigned int animIndex = 0; animIndex < numAnimations; animIndex++) {
		Animation& animation = animationList[animIndex];
		if (!animation.isRunning) {
			continue;
		}

		for (unsigned int nodeIdx = 0; nodeIdx < animation.numNodes; nodeIdx++) {
			AnimationNode& node = animation.nodes[nodeIdx];

			// update the node's matrix...

			for (unsigned int boneIdx = 0; boneIdx < numBones; boneIdx++) {
				Bone& bone = bones[boneIdx];
				if (bone.nodeUniqueId == node.nodeUniqueId) {
					// update the bone's matrix...

					// find the parent bone... multiply its matrix by my matrix

					// do that all the way up the hierarchy...

					// pray its not slow...


				}
			}
		}
	}
}