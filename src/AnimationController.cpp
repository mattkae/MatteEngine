#include "AnimationController.h"
#include "Bone.h"
#include "BinarySerializer.h"
#include "Matrix4x4f.h"
#include "Logger.h"
#include <cassert>

template <typename T>
unsigned int getLerpableValueIdxByTime(GLdouble time, LerpableValue<T>* values, unsigned int numValues) {
	for (unsigned int valueIdx = 0; valueIdx < numValues - 1; valueIdx++) {
		LerpableValue<T> val = values[valueIdx + 1];
		if (time < val.time) {
			return valueIdx;
		}
	}

	logger_error("AnimationController::getLerpableValueByTime, Should never get here");
	return 0;
}

template <typename T>
T lerpValue(GLdouble currentAnimTime, LerpableValue<T>* values, unsigned int numValues) {
	unsigned int startIdx = getLerpableValueIdxByTime(currentAnimTime, values, numValues);
	unsigned int endIdx = startIdx + 1;

	LerpableValue<T> start = values[startIdx];
	LerpableValue<T> end = values[endIdx];

	GLdouble fractionOfTimeThroughFrame = (currentAnimTime - start.time) / (end.time - start.time);
	return start.value.interpolate(end.value, (float)fractionOfTimeThroughFrame);
}

Vector3f AnimationNode::getInterpolatedPosition(GLdouble currentAnimTime) {
	return lerpValue(currentAnimTime, positions, numPositions);
}

Vector3f AnimationNode::getInterpolatedScaling(GLdouble currentAnimTime) {
	return lerpValue(currentAnimTime, scalings, numScalings);
}

Quaternion AnimationNode::getInterpolatedRotation(GLdouble currentAnimTime) {
	return lerpValue(currentAnimTime, rotations, numRotations);
}

void AnimationController::update(float dt, Bone* bones, unsigned int numBones, Matrix4x4f* boneMatrices, Matrix4x4f inverseRootNode, BoneTreeNode* rootNode) {
	for (unsigned int animIndex = 0; animIndex < numAnimations; animIndex++) {
		Animation& animation = animationList[animIndex];
		if (!animation.isRunning) {
			continue;
		}

		animation.currentTimeSeconds += dt;
		GLdouble animationTime = fmod(animation.ticksPerSecond * animation.currentTimeSeconds, animation.duration);

		for (unsigned int nodeIdx = 0; nodeIdx < animation.numNodes; nodeIdx++) {
			AnimationNode& node = animation.nodes[nodeIdx];

			Vector3f position = node.getInterpolatedPosition(animationTime);
			Vector3f scaling = node.getInterpolatedScaling(animationTime);
			Quaternion rotation = node.getInterpolatedRotation(animationTime);

			Bone* bone = &bones[node.boneIndex];
			Matrix4x4f translationMatrix = setTranslation(Matrix4x4f(), position);
			Matrix4x4f rotationMatrix = rotation.normalize().toMatrix();
			Matrix4x4f scalingMatrix = setScale(Matrix4x4f(), scaling);
			bone->localTransform = scalingMatrix * rotationMatrix * translationMatrix;
		}
	}

	if (rootNode != nullptr) {
		rootNode->update(bones, boneMatrices, Matrix4x4f(), inverseRootNode);
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
	serializer.writeFloat32((float)duration);
	serializer.writeFloat32((float)ticksPerSecond);
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
		serializer.writeVec3(positions[positionIdx].value);
		serializer.writeFloat64(positions[positionIdx].time);
	}

	serializer.writeUint32(numScalings);
	for (unsigned int scalingsIdx = 0; scalingsIdx < numScalings; scalingsIdx++) {
		serializer.writeVec3(scalings[scalingsIdx].value);
		serializer.writeFloat64(scalings[scalingsIdx].time);
	}

	serializer.writeUint32(numRotations);
	for (unsigned int rotationIdx = 0; rotationIdx < numRotations; rotationIdx++) {
		serializer.writeQuaternion(rotations[rotationIdx].value);
		serializer.writeFloat64(rotations[rotationIdx].time);
	}
}

void AnimationNode::read(BinarySerializer& serializer) {
	boneIndex = serializer.readUint32();
	numPositions = serializer.readUint32();
	positions = new LerpableValue<Vector3f>[numPositions];
	for (unsigned int positionIdx = 0; positionIdx < numPositions; positionIdx++) {
		positions[positionIdx].value = serializer.readVec3();
		positions[positionIdx].time = serializer.readFloat64();
	}

	numScalings = serializer.readUint32();
	scalings = new LerpableValue<Vector3f>[numScalings];
	for (unsigned int scalingsIdx = 0; scalingsIdx < numScalings; scalingsIdx++) {
		scalings[scalingsIdx].value = serializer.readVec3();
		scalings[scalingsIdx].time = serializer.readFloat64();
	}

	numRotations = serializer.readUint32();
	rotations = new LerpableValue<Quaternion>[numRotations];
	for (unsigned int rotationIdx = 0; rotationIdx < numRotations; rotationIdx++) {
		rotations[rotationIdx].value = serializer.readQuaternion();
		rotations[rotationIdx].time = serializer.readFloat64();
	}
}