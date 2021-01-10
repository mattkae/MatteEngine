#pragma once

#include "../Types.h"
#include "../FixedArray.h"
#include "EntitySystemConstants.h"
#include "../Vector3f.h"
#include "../Box.h"

struct SystemEngine;

struct PhysicsEntity {
	u8 mId = 0;

	Vector3f force = { 0.f, 0.f, 0.f };
	Vector3f acceleration = { 0.f, 0.f, 0.f };
	Vector3f velocity = { 0.f, 0.f, 0.f };
	float mass = 100.f;
	bool isStatic = false;
	Box3D mBox;

	s8 mLastIntersection = -1;

	void applyForceTo(float force, Vector3f direction);
};

struct PhysicsSystem {
	SystemEngine* mSystemEngine;
	FixedArray<PhysicsEntity, MAX_ENTITIES> mEntities;

	void initialize(SystemEngine* systemEngine);
	void update(float dt);
	void free();
};
