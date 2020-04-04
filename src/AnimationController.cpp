#include "AnimationController.h"
#include "Bone.h"
#include "BinarySerializer.h"

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

void Animation::write(BinarySerializer& serializer) {
	serializer.writeFloat32(duration);
	serializer.writeFloat32(ticksPerSecond);
	serializer.writeUint32(numNodes);
}

void Animation::read(BinarySerializer& serializer) {
	duration = serializer.readFloat32();
	ticksPerSecond = serializer.readFloat32();
	numNodes = serializer.readUint32();
}

void AnimationNode::write(BinarySerializer& serializer) {
	serializer.writeUint32(nodeUniqueId);
	serializer.writeUint32(numPositions);
	for (unsigned int positionIdx = 0; positionIdx < numPositions; positionIdx++) {
		serializer.writeVec3(positions[positionIdx]);
	}

	serializer.writeUint32(numScalings);
	for (unsigned int scalingsIdx = 0; scalingsIdx < numScalings; scalingsIdx++) {
		serializer.writeVec3(scalings[scalingsIdx]);
	}

	serializer.writeUint32(numRotations);
	for (unsigned int rotationIdx = 0; rotationIdx < numRotations; rotationIdx++) {
		serializer.writeQuaternion(rotations[rotationIdx]);
	}
}

void AnimationNode::read(BinarySerializer& serializer) {
	nodeUniqueId = serializer.readUint32();
	numPositions = serializer.readUint32();
	positions = new Vector3f[numPositions];
	for (unsigned int positionIdx = 0; positionIdx < numPositions; positionIdx++) {
		positions[positionIdx] = serializer.readVec3();
	}

	numScalings = serializer.readUint32();
	scalings = new Vector3f[numScalings];
	for (unsigned int scalingsIdx = 0; scalingsIdx < numScalings; scalingsIdx++) {
		scalings[scalingsIdx] = serializer.readVec3();
	}

	numRotations = serializer.readUint32();
	rotations = new Quaternion[numRotations];
	for (unsigned int rotationIdx = 0; rotationIdx < numRotations; rotationIdx++) {
		rotations[rotationIdx] = serializer.readQuaternion();
	}
}