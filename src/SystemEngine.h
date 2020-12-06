#pragma once

#include "Types.h"
#include "System/RenderableSystem.h"

const u8 MAX_ENTITIES = 255;

struct SystemEngine {
	u8 mEntityPtr = 0;
	u8 mMaxEntity = 0;
	bool mEntities[MAX_ENTITIES];

	RenderableSystem mRenderSystem;

	u8 registerEntity();
	bool unregisterEntity(u8 ptr);

	void initialize();
	void update(float dtMs);
	void render() const;
	void free();
};