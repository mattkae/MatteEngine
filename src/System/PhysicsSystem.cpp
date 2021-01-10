#include "PhysicsSystem.h"
#include "../SystemEngine.h"

const float gravity = 9.8; // m/s^2

void PhysicsSystem::initialize(SystemEngine* systemEngine) {
	mSystemEngine = systemEngine;
}

void PhysicsSystem::update(float dtMs) {
	FOREACH_FIXED(mEntities) {
		Entity* entity = mSystemEngine->getEntity(value->mId);
		if (entity == nullptr) {
			continue;
		}

		value->currentSpeed += gravity * dtMs;
		entity->mTranslation.y -= value->currentSpeed * dtMs;
	}
}
