#include "MouseInteractable.h"
#include "../ShaderUniformMapping.h"
#include "../SystemEngine.h"
#include "../Input.h"

void MouseInteractableSystem::initialize(SystemEngine* systemEngine, Camera* camera) {
    mSystemEngine = systemEngine;
    mCamera = camera;
}

MouseInteractableEntity* MouseInteractableSystem::castRayToModel() {
    Ray rayWorld = clickToRay(*mCamera);
    GLfloat distanceFromEye = -1;
    MouseInteractableEntity* retval = NULL;

    FOREACH_FIXED(mEntities) {
		if (!value->mIsSelectable) {
			continue;
		}

		const Box3D& box = value->mBox;

		auto renderableEntity = mSystemEngine->mRenderSystem.getEntity(value->mId);
		if (!renderableEntity) {
			continue;
		}
		
		const Model& model = renderableEntity->mModel;
		if (isBoxInRayPath(box, model.model, rayWorld)) {
			GLfloat nextDistanceFromEye = getDistanceFromCamera(box, *mCamera, model.model);
			if (distanceFromEye < 0 || nextDistanceFromEye < distanceFromEye) {
				distanceFromEye = nextDistanceFromEye;
				retval = value;
			}
		}
	}

    return retval;
}

void MouseInteractableSystem::update(float dtMs) {
	if (mSelectedEntity != NULL) {
		if (!isLeftClickDown()) {
		    mSelectedEntity = NULL;
			logger_info("Deselected an entity");
		} 
	} else if (isLeftClickDown()) {
		auto nextSelectEntity = castRayToModel();
		if (nextSelectEntity == NULL) {
			return;
		}

		mSelectedEntity = nextSelectEntity;
		logger_info("Selecting an entity: %d", mSelectedEntity->mId);
    }

	if (mSelectedEntity != NULL) {
		updateBox(mSelectedEntity->mBox);
	}
}

void MouseInteractableSystem::free() {
}

void MouseInteractableSystem::render(const ModelUniformMapping& mapping, bool withMaterial) const {
	if (mSelectedEntity != NULL) {
		Matrix4x4f model = mSystemEngine->mRenderSystem.getEntity(mSelectedEntity->mId)->mModel.model;
		renderBoxOutline(mSelectedEntity->mBox, model, mapping, withMaterial);
	}
}
