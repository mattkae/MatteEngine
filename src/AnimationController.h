#pragma once
#include <GL/glew.h>
#include "Matrix4x4f.h"

struct Vector3f;
struct Quaternion;
struct Bone;

struct AnimationNode {
	unsigned int meshNodeUniqueId;

	int numFrames = 0;
	Vector3f* positions;
	Vector3f* scalings;
	Quaternion* positions;

	Matrix4x4f activeResult;
};

struct Animation {
	bool isRunning = false;
	GLdouble duration;
	GLdouble ticksPerSecond;
	int numNodes = 0;
	AnimationNode* nodes;
};

// The animation controller will live on the Model and update its active animations.
// When an animation is updated, it will update the bones with the same nodeUniqueId.
// Then, at render time, each vertex will index into the 'bones' data on the Model and
// retrieve it's transforms.
struct AnimationController {
	Animation* animationList;
	int numAnimations = 0;
	void update(float dt, Bone* bones, unsigned int numBones);
};