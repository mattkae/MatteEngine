#include "RenderableSystem.h"

void RenderableSystem::initialize() {
	mEntities.allocate(32);
}

void RenderableSystem::update(float dtMs) {
	FOREACH(mEntities) {
		value->mModel.update(dtMs);
	}
}

void RenderableSystem::render(const ModelUniformMapping& mapping, bool withMaterial) const {
	FOREACH(mEntities) {
		value->mModel.render(mapping, withMaterial);
	}
}

void RenderableSystem::free() {
	FOREACH(mEntities) {
		value->mModel.free();
	}

	mEntities.deallocate();
}
