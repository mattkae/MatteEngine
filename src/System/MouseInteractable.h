#pragma once

#include "../Types.h"
#include "../List.h"
#include "../Box.h"
#include "../FixedArray.h"
#include "EntitySystemConstants.h"

struct SystemEngine;
struct Camera;
struct ModelUniformMapping;

struct MouseInteractableEntity {
	u8 mId = 0;
	bool mIsSelectable = true;
	Box3D mBox;
};

struct MouseInteractableSystem {
	Camera* mCamera;
	SystemEngine* mSystemEngine;

	FixedArray<MouseInteractableEntity, MAX_ENTITIES> mEntities;
	MouseInteractableEntity* mSelectedEntity = NULL;

	MouseInteractableEntity* castRayToModel();
	void initialize(SystemEngine* systemEngine, Camera* camera);
	void update(float dtMs);
	void render(const ModelUniformMapping& mapping, bool withMaterial) const;
	void free();
};
