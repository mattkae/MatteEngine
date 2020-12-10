#include "RenderableSystem.h"

void RenderableSystem::initialize() {
}

void RenderableSystem::update(float dtMs) {
	FOREACH_FIXED(mEntities) {
		value->mModel.update(dtMs);
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
}
