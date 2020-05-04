#pragma once
#include <GL/glew.h>
#include "Matrix4x4f.h"

struct Vector3f;
struct Quaternion;
struct Bone;
struct BoneTreeNode;
class BinarySerializer;

template <typename T>
struct LerpableValue {
	T value;
	GLdouble time;
};

struct AnimationNode {
	unsigned int boneIndex = 0;

	unsigned int numPositions = 0;
	LerpableValue<Vector3f>* positions;
	unsigned int numScalings = 0;
	LerpableValue<Vector3f>* scalings;
	unsigned int numRotations = 0;
	LerpableValue<Quaternion>* rotations;

	unsigned int currentPosition = 0;
	unsigned int currentScaling = 0;
	unsigned int currentRotation = 0;

	void write(BinarySerializer& serializer);
	void read(BinarySerializer& serializer);
	Vector3f getInterpolatedPosition(GLdouble currentAnimTime);
	Vector3f getInterpolatedScaling(GLdouble currentAnimTime);
	Quaternion getInterpolatedRotation(GLdouble currentAnimTime);
};

struct Animation {
	bool isRunning = true;
	GLdouble duration = 0; // In seconds
	GLdouble ticksPerSecond = 0;
	unsigned int numNodes = 0;
	AnimationNode* nodes = nullptr;
	GLdouble totalTicks = 0;
	GLdouble currentTimeSeconds = 0;
	GLdouble currentTimeTicks = 0;

	void write(BinarySerializer& serializer);
	void read(BinarySerializer& serializer);
};

struct AnimationController {
	Animation* animationList;
	unsigned int numAnimations = 0;
	void update(float dt, Bone* bones, unsigned int numBones, Matrix4x4f* boneMatrices, Matrix4x4f inverseRootNode, BoneTreeNode* rootNode);
	void free();
};