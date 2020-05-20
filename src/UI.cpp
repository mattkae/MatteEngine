#include "UI.h"
#include "Scene.h"
#include "GlobalApplicationState.h"
#include "Model.h"
#include "SceneUIController.h"

void UI::init() {
	mOrthographicShader = loadShader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	mTextRenderer.initialize(16, (GLchar*)"assets/fonts/consola.ttf");
	panels.allocate(8);
}

void UI::update(double dt) {
	for (size_t idx = 0; idx < panels.numElements; idx++) {
		UIContext& panel = panels[idx];
		if (panel.shouldOpen) {
			for (size_t otherIdx = 0; otherIdx < panels.numElements; otherIdx++) {
				// This only runs once every time you open a new context, so we can afford to be slow but robust
				if (panels[otherIdx].isActive
					&& panels[otherIdx].panel.vertical == panel.panel.vertical
					&& panels[otherIdx].panel.horizontal == panel.panel.horizontal) {
					panels[otherIdx].isActive = false;
				}
			}
		}

		panel.update(mTextRenderer);
	}
}


void UI::render() const {
	useShader(mOrthographicShader);
	Matrix4x4f projection = getOrthographicProjection(0.0f, GlobalAppState.floatWidth, 0.0f, GlobalAppState.floatHeight);
	setShaderMat4(mOrthographicShader, "uProjection", projection);

	for (size_t idx = 0; idx < panels.numElements; idx++) {
		panels[idx].render(mOrthographicShader, mTextRenderer);
	}
}

void UI::free() {
	for (size_t idx = 0; idx < panels.numElements; idx++) {
		UIContext* context = panels.getValue(idx);
		context->free();
	}

	panels.deallocate();

	if (mOrthographicShader > 0) {
		glDeleteProgram(mOrthographicShader);
	}
}

size_t UI::getNextPanelIdx() {
	size_t panelIdx = 0;
	for (; panelIdx < panels.numElements; panelIdx++) {
		if (!panels[panelIdx].isActive) {
			break;
		}
	}

	if (panelIdx == panels.numElements) {
		panels.add(new UIContext());
	}

	return panelIdx;
}

size_t UI::showPanel(UIContext* context, int index) {
	size_t panelIdx = index < 0 ? getNextPanelIdx() : index;
	panels.set(context, panelIdx);
	return panelIdx;
}

size_t UI::showModelSelector(Model* models, size_t numModels) {
	size_t panelIdx = getNextPanelIdx();
	UIContext& context = panels[panelIdx];
	context.shouldOpen = true;
	context.panel.percentageHeight = 0.9f;
	context.panel.percentageWidth = 0.2f;
	context.panel.vertical = UIPositioning::CENTER;
	context.panel.horizontal = UIPositioning::LEFT;
	context.panel.backgroundColor = Vector4f { 0.1, 0.1, 0.1, 0.5 };
	context.panel.borderColor = Vector4f { 0.5, 0.5, 0.5, 0.5 };
	context.panel.borderWidth = 2.f;

	context.uiElements.allocate(numModels);
	for (size_t modelIdx = 0; modelIdx < numModels; modelIdx++) {
		UIElement element;
		Button button;
		button.label = "Model";
		button.buttonColor = Vector4f { 1.0, 0.0, 0.0, 1.0 };
		button.hoverColor = Vector4f { 0.9, 0.1, 0.0, 1.0 };
		button.textColor = Vector4f { 1.0, 1.0, 1.0, 1.0 };
		button.data = modelIdx;
		button.uiRef = controller;
		button.onClick = &SceneUIController::selectModel;
		button.padding = 2.f;
		element.elementType = UIElementType::BUTTON;
		element.element.button = button;
		context.uiElements.add(&element);
	}

	return panelIdx;
}

const size_t numVec3 = 3;

size_t UI::showModelEditor(Model* model) {
	size_t panelIdx = getNextPanelIdx();
	UIContext& context = panels[panelIdx];
	context.shouldOpen = true;
	context.panel.percentageHeight = 0.9f;
	context.panel.percentageWidth = 0.2f;
	context.panel.vertical = UIPositioning::CENTER;
	context.panel.horizontal = UIPositioning::RIGHT;
	context.panel.backgroundColor = Vector4f { 0.1, 0.1, 0.1, 0.5 };
	context.panel.borderColor = Vector4f { 0.5, 0.5, 0.5, 0.5 };
	context.panel.borderWidth = 2.f;

	size_t numElements = 3 * (numVec3 + 1); // Translation, scaling, rotation with labels
	context.uiElements.allocate(numElements);

	{
		UIElement translationLabelElement;
		Label translationLabel;
		translationLabel.bt.padding = 2.f;
		translationLabel.backgroundColor = Vector4f { 1.f, 1.f, 1.f, 1.f };
		translationLabel.textColor = Vector4f { 0.f, 0.f, 0.f , 1.f };
		translationLabel.text = "Translation";
		translationLabelElement.elementType = UIElementType::LABEL;
		translationLabelElement.element.label = translationLabel;
		context.uiElements.add(&translationLabelElement);

		for (unsigned int tIdx = 0; tIdx < numVec3; tIdx++) {
			UIElement element;
			TextInput textInput;
			textInput.bt.padding = 2.f;
			textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
			textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
			textInput.inputType = TextInputType::FLOAT;
			textInput.textColor = Vector4f { 1, 1, 1, 1 };
			textInput.value.fVal = model->translation[tIdx];
			element.elementType = UIElementType::TEXT_INPUT;
			element.element.textInput = textInput;
			context.uiElements.add(&element);
		}
	}

	{
		UIElement scalingLabelElement;
		Label scalingLabel;
		scalingLabel.bt.padding = 2.f;
		scalingLabel.backgroundColor = Vector4f { 1.f, 1.f, 1.f, 1.f };
		scalingLabel.textColor = Vector4f { 0.f, 0.f, 0.f , 1.f };
		scalingLabel.text = "Scale";
		scalingLabelElement.elementType = UIElementType::LABEL;
		scalingLabelElement.element.label = scalingLabel;
		context.uiElements.add(&scalingLabelElement);
		for (unsigned int tIdx = 0; tIdx < numVec3; tIdx++) {
			UIElement element;
			TextInput textInput;
			textInput.bt.padding = 2.f;
			textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
			textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
			textInput.inputType = TextInputType::FLOAT;
			textInput.textColor = Vector4f { 1, 1, 1, 1 };
			textInput.value.fVal = model->scale[tIdx];
			element.elementType = UIElementType::TEXT_INPUT;
			element.element.textInput = textInput;
			context.uiElements.add(&element);
		}
	}

	{
		UIElement rotationLabelElement;
		Label rotationLabel;
		rotationLabel.bt.padding = 2.f;
		rotationLabel.backgroundColor = Vector4f { 1.f, 1.f, 1.f, 1.f };
		rotationLabel.textColor = Vector4f { 0.f, 0.f, 0.f , 1.f };
		rotationLabel.text = "Rotation";
		rotationLabelElement.elementType = UIElementType::LABEL;
		rotationLabelElement.element.label = rotationLabel;
		context.uiElements.add(&rotationLabelElement);
		for (unsigned int tIdx = 0; tIdx < numVec3; tIdx++) {
			UIElement element;
			TextInput textInput;
			textInput.bt.padding = 2.f;
			textInput.backgroundColor = Vector4f { 0.3f, 0.3f, 0.3f, 1 };
			textInput.focusedBackgroundColor = Vector4f { 0.5f, 0.5f, 0.5f, 1 };
			textInput.inputType = TextInputType::FLOAT;
			textInput.textColor = Vector4f { 1, 1, 1, 1 };
			textInput.value.fVal = model->rotation[tIdx];
			element.elementType = UIElementType::TEXT_INPUT;
			element.element.textInput = textInput;
			context.uiElements.add(&element);
		}
	}

	return panelIdx;
}

void UI::hidePanel(size_t index) {
	if (index >= panels.numElements) {
		return;
	}

	panels[index].isActive = false;
}