#pragma once
#include <GL/glew.h>
#include "Matrix4x4f.h"

struct Vector3f;
struct Quaternion;
struct Bone;

struct AnimationNode {
	unsigned int nodeUniqueId;

	int numFrames = 0;
	Vector3f* positions;
	Vector3f* scalings;
	Quaternion* rotations;

	Matrix4x4f activeResult;
};

struct Animation {
	bool isRunning = false;
	GLdouble duration;
	GLdouble ticksPerSecond;
	unsigned int numNodes = 0;
	AnimationNode* nodes;
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
	void update(float dt, Bone* bones, unsigned int numBones);
};