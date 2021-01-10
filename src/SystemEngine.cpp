#include "SystemEngine.h"
#include "Logger.h"
#include "App.h"

u8 SystemEngine::registerEntity() {
	if (mEntityPtr >= MAX_ENTITIES) {
		for (u8 idx = 0; idx < mEntityPtr; idx++) {
			if (!mEntities[idx].mIsActive) {
				mEntities[idx].mIsActive = true;
				mEntities[idx].mId = idx;
				return idx;
			}
		}

		logger_error("Unable to register a new entity");
		return 0;
	}

	Entity newEntity;
	newEntity.mId = mEntityPtr;
	newEntity.mIsActive = true;
	mEntities.add(newEntity);
	mEntityPtr++;
	return newEntity.mId;
}

bool SystemEngine::unregisterEntity(u8 ptr) {
	// @TODO: Important! We need to remove the entity from the child systems
	// later on in life. I don't want to think about this right now, but it will
	// be very important in the future.

	mEntities[ptr].mIsActive = false;
	return true;
}

void SystemEngine::initialize() {
	mRenderSystem.initialize();
	mMouseInteractionSystem.initialize(this, &GlobalApp.scene.mCamera);
}

void SystemEngine::update(float dtMs) {
	mLightSystem.update(dtMs);
	mRenderSystem.update(dtMs);
	mMouseInteractionSystem.update(dtMs);
}

void SystemEngine::render() const {
	
}

void SystemEngine::free() {
	mEntities.clear();
	mEntityPtr = 0;
	mRenderSystem.free();
	mMouseInteractionSystem.free();
	mLightSystem.free();
}
