#include "SystemEngine.h"
#include "Logger.h"
#include "App.h"

Entity* SystemEngine::registerEntity() {
	if (mEntityPtr >= MAX_ENTITIES) {
		for (u8 idx = 0; idx < mEntityPtr; idx++) {
			if (!mEntities[idx].mIsActive) {
				mEntities[idx].mIsActive = true;
				mEntities[idx].mId = idx;
				return mEntities.getValue(idx);
			}
		}

		logger_error("Unable to register a new entity");
		return nullptr;
	}

	Entity newEntity;
	newEntity.mId = mEntityPtr;
	newEntity.mIsActive = true;
	mEntities.add(newEntity);
	mEntityPtr++;
	return mEntities.getValue(mEntityPtr - 1);
}

bool SystemEngine::unregisterEntity(u8 ptr) {
	// @TODO: Important! We need to remove the entity from the child systems
	// later on in life. I don't want to think about this right now, but it will
	// be very important in the future.

	mEntities[ptr].mIsActive = false;
	return true;
}

void SystemEngine::initialize() {
	mRenderSystem.initialize(this);
	mMouseInteractionSystem.initialize(this, &GlobalApp.scene.mCamera);
	mPhysicsSystem.initialize(this);
}

void SystemEngine::update(float dtMs) {
	mLightSystem.update(dtMs);
	mPhysicsSystem.update(dtMs);
	mMouseInteractionSystem.update(dtMs);

	FOREACH_FIXED(mEntities) {
		value->update(dtMs);
	}
	
	mRenderSystem.update(dtMs);
}

void SystemEngine::render() const {
	
}

void SystemEngine::free() {
	mEntities.clear();
	mEntityPtr = 0;
	mRenderSystem.free();
	mMouseInteractionSystem.free();
	mLightSystem.free();
	mPhysicsSystem.free();
}
