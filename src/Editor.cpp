#include "Editor.h"
#include "Input.h"
#include "Scene.h"

List<UI::CreateElementAttribute> getPrimaryElements() {
	List<UI::CreateElementAttribute> retval;

	UI::CreateElementAttribute topLabel = {
		UI::ElementAttribute {
			2.f,
			UI::ElementType_CONTAINER,
			{
				UI::DimensionAttribute {
					UI::Positioning_CENTER,
					UI::Sizing_DYNAMIC,
					0.f, 300.f, 0.5f
				},
				UI::DimensionAttribute {
					UI::Positioning_CENTER,
					UI::Sizing_DYNAMIC,
					0.f, 500.f, 0.5f
				}
			},
			UI::Alignment_LEFT,
			Vector4f { 1.f, 1.f, 1.f, 0.5f },
			Vector4f(),
			UI::FlowType_VERTICAL,
			UI::TextInputType_NONE,
			NULL,
			NULL,
			NULL
		}
	};
	retval.add(topLabel);

	return retval;
}

List<UI::Context> createEditorUI() {
	List<UI::Context> retval;
	retval.add(UI::Context());

	auto primaryElements = getPrimaryElements();

	UI::CreateUI(&retval[0], UI::CreateElementAttribute {
			UI::ElementAttribute {
				2.f, UI::ElementType_CONTAINER,
				{
					UI::DimensionAttribute {
						UI::Positioning_MIN,
						UI::Sizing_STATIC,
						0.f, 300.f, 0.f
					},
					UI::DimensionAttribute {
						UI::Positioning_CENTER,
						UI::Sizing_STATIC,
						0.f, 400.f, 0.f
					}
				},
				UI::Alignment_LEFT,
				Vector4f { 0.3f, 0.1, 0.1, 0.8f },
				Vector4f(),
				UI::FlowType_VERTICAL,
				UI::TextInputType_NONE,
				NULL,
				NULL,
				NULL
			},
			primaryElements
		});

	primaryElements.deallocate();
	return retval;
}

void Editor::initialize(Scene* inScene) {
	mScene = inScene;
	context.initialize();
	context.contextList = createEditorUI();
}

void Editor::free() {	
	mDebugModel.free();
	context.free();
}

int Editor::castRayToModel() {
	Ray rayWorld = clickToRay(mScene->mCamera);

	GLfloat distanceFromEye = -1;
	int retval = -1;

	/*for (size_t mIdx = 0; mIdx < mScene->numModels; mIdx++) {
		const Box3D& box = mScene->modelBoundingBoxes[mIdx];
		const Model& model = mScene->models[mIdx];

		if (isBoxInRayPath(box, model.model, rayWorld)) {
			GLfloat nextDistanceFromEye = getDistanceFromCamera(box, mScene->mCamera, model.model);
			if (distanceFromEye < 0 || nextDistanceFromEye < distanceFromEye) {
				distanceFromEye = nextDistanceFromEye;
				retval = mIdx;
			}
		}
	}*/
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


	if (mSelectedModelIdx >= 0) {
		// @TODO: Do something with the selected model
	}

	context.update(dtMs);
}

void Editor::render() const {
	if (!isEnabled) {
		return;
	}

	if (mSelectedModelIdx >= 0) {
		// @TODO: Do something with the selected model
	}

	context.render();
}
