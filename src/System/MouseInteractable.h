#pragma once

#include "../Types.h"
#include "../List.h"
#include "../Box.h"
#include "../FixedArray.h"
#include "EntitySystemConstants.h"

struct SystemEngine;
struct Camera;

struct MouseInteractableEntity {
	u8 mId = 0;
	bool mIsSelectable = true;
	Box3D mBox;
};

struct MouseInteractableSystem {
	Camera* mCamera;
	SystemEngine* mSystemEngine;

	FixedArray<MouseInteractableEntity, MAX_ENTITIES> mEntities;

	u8 castRayToModel();
	void initialize(SystemEngine* systemEngine, Camera* camera);
	void update(float dtMs);
	void render() const;
	void free();
};