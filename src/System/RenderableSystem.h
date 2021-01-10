#pragma once

#include "../Types.h"
#include "../List.h"
#include "../Model.h"
#include "../ShaderUniformMapping.h"
#include "../Box.h"
#include "EntitySystemConstants.h"
#include "../FixedArray.h"

struct SystemEngine;

struct RenderableEntity {
	u8 mEntityId = 0;
	bool mShouldRender = true;
	Model mModel;
};

struct RenderableSystem {
	FixedArray<RenderableEntity, MAX_ENTITIES> mEntities;
	SystemEngine* mSystemEngine;
	void initialize(SystemEngine* systemEngine);
	void update(float dtMs);
	void render(const ModelUniformMapping& mapping, bool withMaterial = true) const;
	void free();
	RenderableEntity* getEntity(s8 id) {
		FOREACH_FIXED(mEntities) {
			if (value->mEntityId == id) {
				return value;
			}
		}

		return NULL;
	};
};
