#include "MouseInteractable.h"
#include "../SystemEngine.h"
#include "../Input.h"

void MouseInteractableSystem::initialize(SystemEngine* systemEngine, Camera* camera) {
	mSystemEngine = systemEngine;
	mCamera = camera;
}

u8 MouseInteractableSystem::castRayToModel() {
	Ray rayWorld = clickToRay(*mCamera);

	GLfloat distanceFromEye = -1;
	u8 retval = -1;

	FOREACH_FIXED(mEntities) {
		if (!value->mIsSelectable) {
			continue;
		}

		const Box3D& box = value->mBox;
		const Model& model = mSystemEngine->mRenderSystem.mEntities[value->mId].mModel;

		if (isBoxInRayPath(box, model.model, rayWorld)) {
			GLfloat nextDistanceFromEye = getDistanceFromCamera(box, *mCamera, model.model);
			if (distanceFromEye < 0 || nextDistanceFromEye < distanceFromEye) {
				distanceFromEye = nextDistanceFromEye;
				retval = value->mId;
			}
		}
	}

	return retval;
}

void MouseInteractableSystem::update(float dtMs) {
	if (isLeftClickDown()) {
		castRayToModel();
	}
}

void MouseInteractableSystem::free() {
}

void MouseInteractableSystem::render() const {

}