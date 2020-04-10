#pragma once
#include <GL/glew.h>
#include "Matrix4x4f.h"

struct Vector3f;
struct Quaternion;
struct Bone;
class BinarySerializer;

struct AnimationNode {
	unsigned int boneIndex = 0;

	unsigned int numPositions = 0;
	Vector3f* positions;
	unsigned int numScalings = 0;
	Vector3f* scalings;
	unsigned int numRotations = 0;
	Quaternion* rotations;

	unsigned int currentPosition = 0;
	unsigned int currentScaling = 0;
	unsigned int currentRotation = 0;

	void write(BinarySerializer& serializer);
	void read(BinarySerializer& serializer);
};

struct Animation {
	bool isRunning = true;
	GLdouble duration; // In seconds
	GLdouble ticksPerSecond;
	unsigned int numNodes = 0;
	AnimationNode* nodes;

	GLdouble currentTime = 0;
	void write(BinarySerializer& serializer);
	void read(BinarySerializer& serializer);
};

/*
	How animation works:

	Each frame, we calculate a new Matrix for each AnimationNode (which represents
	the point in the animation that we're at). Once calcualted, we find the corresponding
	Bone for that matches the uniqueId in the AnimationNode. This is that particular bone's
	transform, but now we also need to calculate the parent's transform, all the way up
	the node hierarchy.

	Finally, when we update a vertex, we index into the Bones array and get the current transform.
*/
struct AnimationController {
	Animation* animationList;
	unsigned int numAnimations = 0;
	void update(float dt, Bone* bones, unsigned int numBones, Matrix4x4f* boneMatrices);
	void free();
};