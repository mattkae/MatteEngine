#pragma once

#include "../Types.h"
#include "../Light.h"
#include "../FixedArray.h"
#include "EntitySystemConstants.h"

struct Scene;
struct LightUniformMapping;

struct LightEntity {
	u8 mId = 0;
	bool mIsActive = false;
	Light mLight;
};

struct LightSystem {
	FixedArray<LightEntity, MAX_ENTITIES> mEntites;

	void initialize();
	void update(float dtMs);
	void render(const LightUniformMapping* mapping) const;
	void renderShadows(Scene* scene);
	void free();
};