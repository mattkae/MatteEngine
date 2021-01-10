#pragma once

#include "../Types.h"
#include "../FixedArray.h"
#include "EntitySystemConstants.h"

struct SystemEngine;

struct PhysicsEntity {
	u8 mId = 0;
	float currentSpeed = 0;
};

struct PhysicsSystem {
	SystemEngine* mSystemEngine;
	FixedArray<PhysicsEntity, MAX_ENTITIES> mEntities;

	void initialize(SystemEngine* systemEngine);
	void update(float dtMs);
	void free();
};
