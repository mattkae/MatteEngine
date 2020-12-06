#include "Editor.h"
#include "Input.h"
#include "Scene.h"

void Editor::initialize(Scene* inScene) {
	mScene = inScene;
	editorUI.initialize(mScene);
}

void Editor::free() {	
	editorUI.free();
	mDebugModel.free();
}

int Editor::castRayToModel() {
	Ray rayWorld = clickToRay(mScene->mCamera);

	GLfloat distanceFromEye = -1;
	int retval = -1;

	for (size_t mIdx = 0; mIdx < mScene->numModels; mIdx++) {
		const Box3D& box = mScene->modelBoundingBoxes[mIdx];
		const Model& model = mScene->models[mIdx];

		if (isBoxInRayPath(box, model.model, rayWorld)) {
			GLfloat nextDistanceFromEye = getDistanceFromCamera(box, mScene->mCamera, model.model);
			if (distanceFromEye < 0 || nextDistanceFromEye < distanceFromEye) {
				distanceFromEye = nextDistanceFromEye;
				retval = mIdx;
			}
		}
	}
	return retval;
}

void Editor::update(double dtMs) {
	if (!isEnabled) {
		return;
	}

	/*
	if (isLeftClickDown() && isDefaultFocused()) {
		int prevSelection = mSelectedModelIdx;
		mSelectedModelIdx  = castRayToModel();
		if (mSelectedModelIdx > -1) {
			UIEvent selectionEvent = { UIEventType::SHOW_MODEL, &mSelectedModelIdx };
			editorUI.ui.eventProcessor.processEvent(selectionEvent);
		} else if (prevSelection >= 0 && mSelectedModelIdx < 0) {
			UIEvent selectionEvent = { UIEventType::HIDE_MODEL };
			//editorUI.ui.eventProcessor.processEvent(selectionEvent);
		}
	}
	*/

	editorUI.ui.update(dtMs);

	if (mSelectedModelIdx >= 0) {
		// @TODO: Do something with the selected model
	}
}

void Editor::render() const {
	if (!isEnabled) {
		return;
	}

	editorUI.ui.render();

	if (mSelectedModelIdx >= 0) {
		// @TODO: Do something with the selected model
	}
}
