#include "SceneUI.h"
#include "Scene.h"
#include "Logger.h"
#include <string> 

UIContext getModelSelector(const Scene& scene) {
	UIContext retval;

	retval.panel.backgroundColor = glm::vec4(0.1, 0.1, 0.1, 1);
	retval.panel.borderColor = glm::vec4(0.3, 0.3, 0.3, 1);
	retval.panel.percentageWidth = 0.25f;
	retval.panel.percentageHeight = 0.75f;
	retval.panel.horizontal = UIPositioning::LEFT;
	retval.panel.vertical = UIPositioning::CENTER;
	retval.panel.borderWidth = 1.f;

	allocateArray(retval.uiElements, scene.models.size());

	for (size_t elementIndex = 0; elementIndex < retval.uiElements.numElements; elementIndex++) {
		Button button;
		button.label = std::to_string(elementIndex);
		button.onClickHandler = [button]() { Logger::logInfo(button.label); };
		button.buttonColor = glm::vec4(0, 1, 0, 1);
		button.padding = 2.f;
		retval.uiElements.elements[elementIndex].element = button;
		retval.uiElements.elements[elementIndex].type = UIElement::BUTTON;
	}

	retval.spaceBetweenElements = 2.f;

	return retval;
}