#include "SceneUI.h"
#include "Scene.h"
#include "Logger.h"
#include <string>

void onChange(Scene& scene, size_t elementIndex, size_t translationComponentIndex, std::string value) {
	if (value.length() == 0) {
		return;
	}

	scene.models.at(elementIndex).model[3][translationComponentIndex] = std::stof(value);
}

UIContext getModelEditor(Scene& scene, UIContext& modelContext, size_t elementIndex) {
	UIContext retval;
	retval.panel.backgroundColor = glm::vec4(0.1, 0.1, 0.1, 1);
	retval.panel.borderColor = glm::vec4(0.3, 0.3, 0.3, 1);
	retval.panel.percentageWidth = 0.15f;
	retval.panel.percentageHeight = 0.75f;
	retval.panel.horizontal = UIPositioning::RIGHT;
	retval.panel.vertical = UIPositioning::CENTER;
	retval.panel.borderWidth = 1.f;
	retval.isActive = true;

	allocateArray(retval.uiElements, 3);

	for (size_t translationComponentIndex = 0; translationComponentIndex < retval.uiElements.numElements; translationComponentIndex++) {
		TextInput textInput;

		textInput.str = std::to_string(scene.models.at(elementIndex).model[3][translationComponentIndex]);
		textInput.textColor = glm::vec4(0, 0, 1, 1);
		textInput.backgroundColor = glm::vec4(0, 1, 0, 1);
		textInput.focusedBackgroundColor = glm::vec4(1, 0, 0, 1);
		textInput.onChangeHandler = [&scene, elementIndex, translationComponentIndex](std::string value) mutable { onChange(scene, elementIndex, translationComponentIndex, value); };

		initializeTextInput(textInput);
		retval.uiElements.elements[translationComponentIndex].element = textInput;
		retval.uiElements.elements[translationComponentIndex].type = UIElement::TEXT_INPUT;
	}

	retval.spaceBetweenElements = 2.f;

	if (modelContext.dependentContexts.elements[0].isActive) {
		deallocateArray(modelContext.dependentContexts.elements[0].uiElements);
	}

	modelContext.dependentContexts.elements[0] = retval;

	return retval;
}

void getModelSelector(Scene& scene, UIContext &retval) {
	retval.panel.backgroundColor = glm::vec4(0.1, 0.1, 0.1, 1);
	retval.panel.borderColor = glm::vec4(0.3, 0.3, 0.3, 1);
	retval.panel.percentageWidth = 0.15f;
	retval.panel.percentageHeight = 0.75f;
	retval.panel.horizontal = UIPositioning::LEFT;
	retval.panel.vertical = UIPositioning::CENTER;
	retval.panel.borderWidth = 1.f;

	allocateArray(retval.uiElements, scene.models.size());
	allocateArray(retval.dependentContexts, 1);

	for (size_t elementIndex = 0; elementIndex < retval.uiElements.numElements; elementIndex++) {
		Button button;
		button.label = std::to_string(elementIndex);
		button.onClickHandler = [&scene, elementIndex, &retval]() mutable { getModelEditor(scene, retval, elementIndex); };
		button.buttonColor = glm::vec4(0, 1, 0, 1);
		button.padding = 2.f;
		retval.uiElements.elements[elementIndex].element = button;
		retval.uiElements.elements[elementIndex].type = UIElement::BUTTON;
	}

	retval.spaceBetweenElements = 2.f;
	retval.dependentContexts.elements[0].isActive = false;
}