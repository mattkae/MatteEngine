#include "Editor.h"
#include "Input.h"
#include "Scene.h"
#include "App.h"

void turnGreen(UI::Context* context) {
	context->attr.backgroundColor = Vector4f { 0, 1, 0, 1 };
	context->attr.textColor = Vector4f { 1, 0, 0, 1 };
}

void turnRed(UI::Context* context) {
	context->attr.backgroundColor = Vector4f { 1, 0, 0, 1 };
	context->attr.textColor = Vector4f { 0, 1, 0, 1 };
}

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
					UI::Alignment_CENTER,
					0.f, 300.f, 0.9f
				},
				UI::DimensionAttribute {
					UI::Positioning_FLOW,
					UI::Sizing_DYNAMIC,
					UI::Alignment_CENTER,
					0.f, 500.f, 0.1f
				}
			},
			Vector4f { 1.f, 1.f, 1.f, 0.5f },
			Vector4f(),
			UI::FlowType_NONE,
			UI::TextInputType_NONE
		},
		(char*)"Hello World"
	};
	retval.add(topLabel);

	UI::CreateElementAttribute firstButton = {
		UI::ElementAttribute {
			2.f,
			UI::ElementType_INPUT,
			{
				UI::DimensionAttribute {
					UI::Positioning_CENTER,
					UI::Sizing_DYNAMIC,
					UI::Alignment_CENTER,
					0.f, 300.f, 0.9f
				},
				UI::DimensionAttribute {
					UI::Positioning_FLOW,
					UI::Sizing_DYNAMIC,
					UI::Alignment_CENTER,
					0.f, 500.f, 0.1f
				}
			},
			Vector4f { 1.f, 1.f, 1.f, 0.5f },
			Vector4f(),
			UI::FlowType_NONE,
			UI::TextInputType_NONE
		},
		(char*)"Hello World"
	};
	retval.add(firstButton);

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
						UI::Alignment_CENTER,
						0.f, 300.f, 0.f
					},
					UI::DimensionAttribute {
						UI::Positioning_CENTER,
						UI::Sizing_STATIC,
						UI::Alignment_CENTER,
						0.f, 400.f, 0.f
					}
				},
				Vector4f { 0.3f, 0.1, 0.1, 0.8f },
				Vector4f(),
				UI::FlowType_IN_FLOW,
				UI::TextInputType_NONE
			},
			NULL,
			primaryElements
		});

	primaryElements.deallocate();
	return retval;
}

void onTerminalTextChanged(UI::Context* context) {
	printf("%s\n", context->content.getValue());
}

List<UI::CreateElementAttribute> getTerminalElements() {
	List<UI::CreateElementAttribute> retval;

	UI::CreateElementAttribute input = {
		UI::ElementAttribute {
			2.f,
			UI::ElementType_INPUT,
			{
				UI::DimensionAttribute {
					UI::Positioning_CENTER,
					UI::Sizing_DYNAMIC,
					UI::Alignment_LEFT,
					0.f, 0.f, 0.9f
				},
				UI::DimensionAttribute {
					UI::Positioning_MIN,
					UI::Sizing_STATIC,
					UI::Alignment_CENTER,
					0.f, 16.f, 0.f
				}
			},
			Vector4f { 1.f, 1.f, 1.f, 0.5f },
			Vector4f(),
			UI::FlowType_NONE,
			UI::TextInputType_STRING,
			onTerminalTextChanged
		},
		(char*)"Enter a Terminal Command"
	};
	retval.add(input);

	return retval;
}

List<UI::Context> createTerminal() {
	List<UI::Context> retval;
	retval.add(UI::Context());

	auto elements = getTerminalElements();

	UI::CreateUI(&retval[0], UI::CreateElementAttribute {
			UI::ElementAttribute {
				2.f, UI::ElementType_CONTAINER,
				{
					UI::DimensionAttribute {
						UI::Positioning_CENTER,
						UI::Sizing_DYNAMIC,
						UI::Alignment_CENTER,
						0.f, 0.f, 0.75f
					},
					UI::DimensionAttribute {
						UI::Positioning_MAX,
						UI::Sizing_STATIC,
						UI::Alignment_CENTER,
						0.f, 250.f, 0.f
					}
				},
				Vector4f { 0.3f, 0.3f, 0.3f, 0.5f },
				Vector4f(),
				UI::FlowType_IN_FLOW,
				UI::TextInputType_NONE
			},
			NULL,
			elements
		});

	elements.deallocate();
	return retval;
}

void Editor::initialize(Scene* inScene) {
	mScene = inScene;
	mainContext.initialize();
	mainContext.contextList = createEditorUI();

	terminalContext.initialize();
	terminalContext.contextList = createTerminal();
}

void Editor::free() {	
	mDebugModel.free();
	mainContext.free();
	terminalContext.free();
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

	mainContext.update(dtMs);
	terminalContext.update(dtMs);
}

void Editor::render() const {
	if (!isEnabled) {
		return;
	}

	if (mSelectedModelIdx >= 0) {
		// @TODO: Do something with the selected model
	}

	mainContext.render();
	terminalContext.render();
}
