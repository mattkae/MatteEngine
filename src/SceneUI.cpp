#include "SceneUI.h"
#include "Scene.h"
#include "UI.h"
#include "Logger.h"
#include <string>

void getMainEditor(BetterScene& scene, UIContext& retVal, UI& ui);
void getModelEditor(Model& model, UIContext& retval, size_t elementIdx);

void getSceneUI(BetterScene& scene, UI& ui) {
	size_t numRightPanels = scene.numModels;
	size_t numMainUiElements = 1 + numRightPanels;
	size_t uiPanelIndex = 0;

	ui.panels = new UIContext[numMainUiElements];
	ui.numPanels = numMainUiElements;
	getMainEditor(scene,  ui.panels[uiPanelIndex++], ui);

	ui.modelOffset = uiPanelIndex;

	// Model editor panels
	for (size_t mIdx = 0; mIdx < scene.numModels; mIdx++) {
		UIContext& modelContext = ui.panels[uiPanelIndex];
		getModelEditor(scene.models[mIdx], modelContext, mIdx);
		uiPanelIndex++;
	}
}

void getMainEditor(BetterScene& scene, UIContext& retVal, UI& ui) {
	retVal.uiElements = new UIElement[scene.numModels + 1];
	retVal.numUiElements = scene.numModels + 1;
	size_t elementIdx = 0;

	retVal.panel.backgroundColor = getVec4(0.1, 0.1, 0.1);
	retVal.panel.borderColor = getVec4(0.3, 0.3, 0.3);
	retVal.panel.percentageWidth = 0.15f;
	retVal.panel.percentageHeight = 0.75f;
	retVal.panel.horizontal = UIPositioning::LEFT;
	retVal.panel.vertical = UIPositioning::CENTER;
	retVal.panel.borderWidth = 1.f;
	retVal.isActive = true;

	// Model Selector
	Label modelLabel;
	modelLabel.bt.padding = 2.f;
	modelLabel.text = "Models";
	modelLabel.backgroundColor = Vector4f();
	modelLabel.textColor = getVec4(1);
	retVal.uiElements[elementIdx].type = UIElement::LABEL;
	retVal.uiElements[elementIdx].element = modelLabel;
	retVal.spaceBetweenElements = 2.f;
	elementIdx++;

	for (size_t mIdx = 0; mIdx < scene.numModels; mIdx++) {
		const size_t modelPanelIdx = mIdx + 1;
		Button button;
		button.label = "Model #" + std::to_string(mIdx + 1);
		button.buttonColor = getVec4(0.9, 0.9, 0.9);
		button.hoverColor = getVec4(1);
		button.padding = 2.f;
		button.isClicked = &ui.panels[modelPanelIdx].shouldOpen;
		retVal.uiElements[elementIdx].element = button;
		retVal.uiElements[elementIdx].type = UIElement::BUTTON;
		elementIdx++;
	}
}

void getModelEditor(Model& model, UIContext& retval, size_t elementIdx) {
	retval.panel.backgroundColor = getVec4(0.1, 0.1, 0.1);
	retval.panel.borderColor = getVec4(0.3, 0.3, 0.3);
	retval.panel.percentageWidth = 0.15f;
	retval.panel.percentageHeight = 0.75f;
	retval.panel.horizontal = UIPositioning::RIGHT;
	retval.panel.vertical = UIPositioning::CENTER;
	retval.panel.borderWidth = 1.f;
	retval.isActive = false;
	retval.uiElements = new UIElement[4];
	retval.numUiElements = 4;

	// Model Label
	Label modelLabel;
	modelLabel.bt.padding = 2.f;
	modelLabel.text = "Model #" + std::to_string(elementIdx + 1);
	modelLabel.backgroundColor = Vector4f();
	modelLabel.textColor = getVec4(1);
	retval.uiElements[0].type = UIElement::LABEL;
	retval.uiElements[0].element = modelLabel;

	// Translation editor
	for (size_t translationComponentIndex = 0; translationComponentIndex < 3; translationComponentIndex++) {
		TextInput textInput;

		textInput.isFocused = false;
		textInput.inputType = TextInputType::FLOAT;
		textInput.value.fVal = &model.model.values[12 + translationComponentIndex];
		textInput.textColor = getVec4(0, 0, 0);
		textInput.backgroundColor = getVec4(0.9, 0.9, 0.9);
		textInput.focusedBackgroundColor = getVec4(1, 1, 1);
		textInput.bt.padding = 2.f;

		retval.uiElements[translationComponentIndex + 1].element = textInput;
		retval.uiElements[translationComponentIndex + 1].type = UIElement::TEXT_INPUT;
	}

	retval.spaceBetweenElements = 2.f;
}