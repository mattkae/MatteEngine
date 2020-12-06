#include "SystemEngine.h"
#include "Logger.h"

u8 SystemEngine::registerEntity() {
	if (mMaxEntity >= MAX_ENTITIES) {
		for (u8 idx = 0; idx < mMaxEntity; idx++) {
			if (!mEntities[idx]) {
				mEntities[idx] = true;
				return idx;
			}
		}

		logger_error("Unable to register a new entity");
		return 0;
	}

	u8 retval = mEntityPtr;
	mEntities[retval] = true;
	mEntityPtr++;
	mMaxEntity++;
	return retval;
}

bool SystemEngine::unregisterEntity(u8 ptr) {
	mEntities[ptr] = false;
	return true;
}

void SystemEngine::initialize() {
	mRenderSystem.initialize();
}

void SystemEngine::update(float dtMs) {
	for (u8 idx = 0; idx < mMaxEntity; idx++) {
		if (!mEntities[idx]) {
			return;
		}

	}

	mRenderSystem.update(dtMs);
}

void SystemEngine::free() {
	mRenderSystem.free();
}