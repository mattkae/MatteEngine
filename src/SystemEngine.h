#pragma once

#include "Types.h"
#include "System/RenderableSystem.h"
#include "System/MouseInteractable.h"
#include "System/EntitySystemConstants.h"
#include "System/LightSystem.h"
#include "Vector3f.h"
#include "FixedArray.h"

struct Entity {
	u8 mId = 0;
	bool mIsActive = false;
	Vector3f mPosition = { 0, 0, 0 };
};

struct SystemEngine {
	u8 mEntityPtr = 0;
	FixedArray<Entity, MAX_ENTITIES> mEntities;

	LightSystem mLightSystem;
	RenderableSystem mRenderSystem;
	MouseInteractableSystem mMouseInteractionSystem;

	u8 registerEntity();
	bool unregisterEntity(u8 ptr);
	inline Entity* getEntity(u8 id) {
		if (id >= mEntityPtr) {
			return nullptr;
		}

		if (!mEntities[id].mIsActive) {
			return nullptr;
		}

		return &mEntities[id];
	}

	void initialize();
	void update(float dtMs);
	void render() const;
	void free();
};