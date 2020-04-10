#include "AnimationController.h"
#include "Bone.h"
#include "BinarySerializer.h"

unsigned int calculateTickIndex(GLdouble tickPosition, GLdouble totalTicks, unsigned int numIndices) {
	return floor((tickPosition / totalTicks) * numIndices);
}

void propagateBoneTransform(Bone bone, Bone* bones, Vector3f position, Vector3f scaling, Quaternion rotation) {
	for (unsigned int childIdx = 0; childIdx < bone.numChildBones; childIdx++) {
		Bone& child = bones[bone.childrenBoneIndices[childIdx]];
		child.position = child.position + position;
		child.scaling = child.scaling + scaling;
		child.rotation = child.rotation * rotation;

		propagateBoneTransform(child, bones, position, scaling, rotation);
	}
}

void AnimationController::update(float dt, Bone* bones, unsigned int numBones, Matrix4x4f* boneMatrices) {
	for (unsigned int animIndex = 0; animIndex < numAnimations; animIndex++) {
		Animation& animation = animationList[animIndex];
		if (!animation.isRunning) {
			continue;
		}

		animation.currentTime += dt;
		if (animation.currentTime >= animation.duration) {
			animation.currentTime = 0;
		}

		GLdouble tick = animation.currentTime * animation.ticksPerSecond;
		GLdouble totalTicks = animation.duration * animation.ticksPerSecond;

		for (unsigned int nodeIdx = 0; nodeIdx < animation.numNodes; nodeIdx++) {
			AnimationNode& node = animation.nodes[nodeIdx];

			Vector3f position = node.positions[calculateTickIndex(tick, totalTicks, node.numPositions)];
			Vector3f scaling = node.scalings[calculateTickIndex(tick, totalTicks, node.numScalings)];
			Quaternion rotation = node.rotations[calculateTickIndex(tick, totalTicks, node.numRotations)];

			Bone& bone = bones[node.boneIndex];
			bone.position = position;
			bone.scaling = scaling;
			bone.rotation = rotation;
			propagateBoneTransform(bone, bones, position, scaling, rotation);
		}
	}
}

void AnimationController::free() {
	if (animationList == nullptr) {
		return;
	}

	for (unsigned int animationIdx = 0; animationIdx < numAnimations; animationIdx++) {
		Animation* animation = &animationList[animationIdx];
		for (unsigned int nodeIdx = 0; nodeIdx < animation->numNodes; nodeIdx++) {
			AnimationNode* node = &animation->nodes[nodeIdx];
			delete node->positions;
			delete node->rotations;
			delete node->scalings;
		}

		delete animation->nodes;
	}

	delete animationList;
}

// ***************************************
// Binary Serializations
// ***************************************
void Animation::write(BinarySerializer& serializer) {
	serializer.writeFloat32(duration);
	serializer.writeFloat32(ticksPerSecond);
	serializer.writeUint32(numNodes);
	for (unsigned int nodeIdx = 0; nodeIdx < numNodes; nodeIdx++) {
		nodes[nodeIdx].write(serializer);
	}
}

void Animation::read(BinarySerializer& serializer) {
	duration = serializer.readFloat32();
	ticksPerSecond = serializer.readFloat32();
	numNodes = serializer.readUint32();
	nodes = new AnimationNode[numNodes];
	for (unsigned int nodeIdx = 0; nodeIdx < numNodes; nodeIdx++) {
		nodes[nodeIdx].read(serializer);
	}
}

void AnimationNode::write(BinarySerializer& serializer) {
	serializer.writeUint32(boneIndex);
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
	boneIndex = serializer.readUint32();
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