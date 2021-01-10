#include "RenderableSystem.h"
#include "../SystemEngine.h"

void RenderableSystem::initialize(SystemEngine* systemEngine) {
	mSystemEngine = systemEngine;
}

void RenderableSystem::update(float dtMs) {
	FOREACH_FIXED(mEntities) {
		Entity* entity = mSystemEngine->getEntity(value->mEntityId);
		if (entity == nullptr) {
			logger_error("Failed to find entity with id=%u", value->mEntityId);
			continue;
		}
		
		value->mModel.update(dtMs, entity->mModel);
	}
}

void RenderableSystem::render(const ModelUniformMapping& mapping, bool withMaterial) const {
	FOREACH_FIXED_CONST(mEntities) {
		value->mModel.render(mapping, withMaterial);
	}
}

void RenderableSystem::free() {
	FOREACH_FIXED(mEntities) {
		value->mModel.free();
	}
	mEntities.clear();
}
