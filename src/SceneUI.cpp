#include "SceneUI.h"
#include "Scene.h"
#include "UI/UI.h"
#include "Logger.h"
#include <string>

void getModelEditor(Model& model, UIContext& retval, size_t elementIdx);

void getSceneUI(BetterScene& scene, UI& ui) {
	allocateArray(ui.context, 1 + scene.numModels);

	// Model editor panels
	for (size_t mIdx = 0; mIdx < scene.numModels; mIdx++) {
		allocateArray(ui.context.elements[mIdx].deactivationDeps, scene.numModels);
		for (size_t dIdx = 0; dIdx < scene.numModels; dIdx++) {
			ui.context.elements[mIdx].deactivationDeps.elements[dIdx] = &ui.context.elements[dIdx];
		}

		getModelEditor(scene.models[mIdx], ui.context.elements[mIdx], mIdx);
	}

	// Model Selector Panel
	ui.context.elements[scene.numModels].panel.backgroundColor = glm::vec4(0.1, 0.1, 0.1, 1);
	ui.context.elements[scene.numModels].panel.borderColor = glm::vec4(0.3, 0.3, 0.3, 1);
	ui.context.elements[scene.numModels].panel.percentageWidth = 0.15f;
	ui.context.elements[scene.numModels].panel.percentageHeight = 0.75f;
	ui.context.elements[scene.numModels].panel.horizontal = UIPositioning::LEFT;
	ui.context.elements[scene.numModels].panel.vertical = UIPositioning::CENTER;
	ui.context.elements[scene.numModels].panel.borderWidth = 1.f;
	ui.context.elements[scene.numModels].isActive = true;

	allocateArray(ui.context.elements[scene.numModels].uiElements, scene.numModels + 1);
	Label modelLabel;
	modelLabel.bt.padding = 2.f;
	modelLabel.text = "Models";
	modelLabel.backgroundColor = glm::vec4(0);
	modelLabel.textColor = glm::vec4(1);
	ui.context.elements[scene.numModels].uiElements.elements[0].type = UIElement::LABEL;
	ui.context.elements[scene.numModels].uiElements.elements[0].element = modelLabel;

	for (size_t mIdx = 0; mIdx < scene.numModels; mIdx++) {
		UIContext& modelContext = ui.context.elements[mIdx];
		Button button;
		button.label = "Model #" + std::to_string(mIdx + 1);
		button.buttonColor = glm::vec4(0.9, 0.9, 0.9, 1);
		button.hoverColor = glm::vec4(1, 1, 1, 1);
		button.padding = 2.f;
		button.isClicked = &modelContext.shouldOpen;
		ui.context.elements[scene.numModels].uiElements.elements[mIdx + 1].element = button;
		ui.context.elements[scene.numModels].uiElements.elements[mIdx + 1].type = UIElement::BUTTON;
	}

	ui.context.elements[scene.numModels].spaceBetweenElements = 2.f;
}

void getModelEditor(Model& model, UIContext& retval, size_t elementIdx) {
	retval.panel.backgroundColor = glm::vec4(0.1, 0.1, 0.1, 1);
	retval.panel.borderColor = glm::vec4(0.3, 0.3, 0.3, 1);
	retval.panel.percentageWidth = 0.15f;
	retval.panel.percentageHeight = 0.75f;
	retval.panel.horizontal = UIPositioning::RIGHT;
	retval.panel.vertical = UIPositioning::CENTER;
	retval.panel.borderWidth = 1.f;
	retval.isActive = false;
	allocateArray(retval.uiElements, 4);

	// Model Label
	Label modelLabel;
	modelLabel.bt.padding = 2.f;
	modelLabel.text = "Model #" + std::to_string(elementIdx + 1);
	modelLabel.backgroundColor = glm::vec4(0);
	modelLabel.textColor = glm::vec4(1);
	retval.uiElements.elements[0].type = UIElement::LABEL;
	retval.uiElements.elements[0].element = modelLabel;

	// Translation editor
	for (size_t translationComponentIndex = 0; translationComponentIndex < 3; translationComponentIndex++) {
		TextInput textInput;

		textInput.isFocused = false;
		textInput.inputType = TextInputType::FLOAT;
		textInput.value.fVal = &model.model[3][translationComponentIndex];
		textInput.textColor = glm::vec4(0, 0, 0, 1);
		textInput.backgroundColor = glm::vec4(0.9, 0.9, 0.9, 1);
		textInput.focusedBackgroundColor = glm::vec4(1, 1, 1, 1);
		textInput.bt.padding = 2.f;

		retval.uiElements.elements[translationComponentIndex + 1].element = textInput;
		retval.uiElements.elements[translationComponentIndex + 1].type = UIElement::TEXT_INPUT;
	}

	retval.spaceBetweenElements = 2.f;
}