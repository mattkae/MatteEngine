#pragma once

#include "Types.h"
#include "System/RenderableSystem.h"
#include "System/MouseInteractable.h"
#include "System/EntitySystemConstants.h"
#include "System/LightSystem.h"
#include "System/PhysicsSystem.h"
#include "Vector3f.h"
#include "Quaternion.h"
#include "Matrix4x4f.h"
#include "FixedArray.h"

struct Entity {
	u8 mId = 0;
	bool mIsActive = false;

	// @TODO: We need to use this instead of the various
	// attributes in Model and Light etc for these universal
	// values
	Vector3f mTranslation = { 0, 0, 0 };
	Vector3f mScale = { 1.f, 1.f, 1.f };
	Quaternion mRotation;
    Matrix4x4f mModel;

	void update(float dtMs) {
		 Matrix4x4f translationMatrix = setTranslation(Matrix4x4f(), mTranslation);
		 Matrix4x4f rotationMatrix = mRotation.normalize().toMatrix();
		 Matrix4x4f scalingMatrix = setScale(Matrix4x4f(), mScale);
		 mModel = scalingMatrix * rotationMatrix * translationMatrix;// TODO: What's this - defaultModel;
	}
};

struct SystemEngine {
	u8 mEntityPtr = 0;
	FixedArray<Entity, MAX_ENTITIES> mEntities;

	LightSystem mLightSystem;
	RenderableSystem mRenderSystem;
	MouseInteractableSystem mMouseInteractionSystem;
	PhysicsSystem mPhysicsSystem;

	Entity* registerEntity();
	bool unregisterEntity(u8 ptr);
	inline Entity* getEntity(u8 id) {
		if (id >= mEntityPtr) {
			logger_error("Entity exceeded mEntityPtr: %u > %u", id, mEntityPtr);
			return nullptr;
		}

		if (!mEntities[id].mIsActive) {
			logger_error("Entity is no longer active: %u", id);
			return nullptr;
		}

		return &mEntities[id];
	}

	void initialize();
	void update(float dtMs);
	void render() const;
	void free();
};
