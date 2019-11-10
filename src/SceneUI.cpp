#include "SceneUI.h"
#include "Scene.h"
#include "UI/UI.h"
#include "Logger.h"
#include <string>

void getMainEditor(BetterScene& scene, UIContext& retVal, UI& ui);
void getModelEditor(Model& model, UIContext& retval, size_t elementIdx);
void getSphereEditor(Sphere& sphere, UIContext& retval, size_t elementIdx);

void getSceneUI(BetterScene& scene, UI& ui) {
	size_t numRightPanels = scene.numModels + scene.numSpheres;
	size_t numMainUiElements = 1 + numRightPanels;
	size_t uiContextIndex = 0;

	allocateArray(ui.context, numMainUiElements);
	getMainEditor(scene,  ui.context.elements[uiContextIndex++], ui);

	// Model editor panels
	for (size_t mIdx = 0; mIdx < scene.numModels; mIdx++) {
		UIContext& modelContext = ui.context.elements[uiContextIndex];
		allocateArray(modelContext.deactivationDeps, numRightPanels);
		for (size_t dIdx = 1; dIdx <= numRightPanels; dIdx++) {
			modelContext.deactivationDeps.elements[dIdx - 1] = &ui.context.elements[dIdx];
		}

		getModelEditor(scene.models[mIdx], modelContext, mIdx);
		uiContextIndex++;
	}

	// Sphere editor panels
	for (size_t sIdx = 0; sIdx < scene.numSpheres; sIdx++) {
		UIContext& sphereContext = ui.context.elements[uiContextIndex++];
		allocateArray(sphereContext.deactivationDeps, numRightPanels);
		for (size_t dIdx = 1; dIdx <= numRightPanels; dIdx++) {
			sphereContext.deactivationDeps.elements[dIdx - 1] = &ui.context.elements[dIdx];
		}

		getSphereEditor(scene.spheres[sIdx], sphereContext, sIdx);
	}
}

void getMainEditor(BetterScene& scene, UIContext& retVal, UI& ui) {
	allocateArray(retVal.uiElements, scene.numModels + 1 + scene.numSpheres + 1);
	size_t elementIdx = 0;

	retVal.panel.backgroundColor = glm::vec4(0.1, 0.1, 0.1, 1);
	retVal.panel.borderColor = glm::vec4(0.3, 0.3, 0.3, 1);
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
	modelLabel.backgroundColor = glm::vec4(0);
	modelLabel.textColor = glm::vec4(1);
	retVal.uiElements.elements[elementIdx].type = UIElement::LABEL;
	retVal.uiElements.elements[elementIdx].element = modelLabel;
	retVal.spaceBetweenElements = 2.f;
	elementIdx++;

	for (size_t mIdx = 0; mIdx < scene.numModels; mIdx++) {
		const size_t modelPanelIdx = mIdx + 1;
		UIContext& modelContext = ui.context.elements[modelPanelIdx];
		Button button;
		button.label = "Model #" + std::to_string(mIdx + 1);
		button.buttonColor = glm::vec4(0.9, 0.9, 0.9, 1);
		button.hoverColor = glm::vec4(1, 1, 1, 1);
		button.padding = 2.f;
		button.isClicked = &modelContext.shouldOpen;
		retVal.uiElements.elements[elementIdx].element = button;
		retVal.uiElements.elements[elementIdx].type = UIElement::BUTTON;
		elementIdx++;
	}

	// Sphere selector
	Label sphereLabel;
	sphereLabel.bt.padding = 2.f;
	sphereLabel.text = "Spheres";
	sphereLabel.backgroundColor = glm::vec4(0);
	sphereLabel.textColor = glm::vec4(1);
	retVal.uiElements.elements[elementIdx].type = UIElement::LABEL;
	retVal.uiElements.elements[elementIdx].element = sphereLabel;
	retVal.spaceBetweenElements = 2.f;
	elementIdx++;

	for (size_t sIdx = 0; sIdx < scene.numSpheres; sIdx++) {
		const size_t spherePanelIdx = 1 + scene.numModels + sIdx;
		UIContext& sphereContext = ui.context.elements[spherePanelIdx];
		Button button;
		button.label = "Sphere #" + std::to_string(sIdx + 1);
		button.buttonColor = glm::vec4(0.9, 0.9, 0.9, 1);
		button.hoverColor = glm::vec4(1, 1, 1, 1);
		button.padding = 2.f;
		button.isClicked = &sphereContext.shouldOpen;
		retVal.uiElements.elements[elementIdx].element = button;
		retVal.uiElements.elements[elementIdx].type = UIElement::BUTTON;
		elementIdx++;
	}
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

void getSphereEditor(Sphere& sphere, UIContext& retval, size_t elementIdx) {
	retval.panel.backgroundColor = glm::vec4(0.1, 0.1, 0.1, 1);
	retval.panel.borderColor = glm::vec4(0.3, 0.3, 0.3, 1);
	retval.panel.percentageWidth = 0.15f;
	retval.panel.percentageHeight = 0.75f;
	retval.panel.horizontal = UIPositioning::RIGHT;
	retval.panel.vertical = UIPositioning::CENTER;
	retval.panel.borderWidth = 1.f;
	retval.isActive = false;
	allocateArray(retval.uiElements, 1);

	// Sphere Label
	Label modelLabel;
	modelLabel.bt.padding = 2.f;
	modelLabel.text = "Sphere #" + std::to_string(elementIdx + 1);
	modelLabel.backgroundColor = glm::vec4(0);
	modelLabel.textColor = glm::vec4(1);
	retval.uiElements.elements[0].type = UIElement::LABEL;
	retval.uiElements.elements[0].element = modelLabel;
}