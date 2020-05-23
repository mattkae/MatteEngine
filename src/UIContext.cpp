#include "UIContext.h"
#include "GlobalApplicationState.h"

void UIContext::update(const TextRenderer& textRenderer, UIEvent& event) {
	if (shouldOpen && !isActive) {
		isActive = true;
		shouldOpen = false;
	} else if (shouldClose && isActive) {
		shouldClose = false;
		isActive = false;
	}

	if (!isActive) {
		return;
	}

	panel.update();
	GLfloat yOffset = GlobalAppState.floatHeight - panel.boundingRect.y - panel.padding;
	GLfloat xPosition = panel.boundingRect.x + panel.padding;
	
	for (size_t elementIndex = 0; elementIndex < uiElements.numElements; elementIndex++) {
		UIElement& element = uiElements[elementIndex];
		switch (element.elementType) {
		case UIElementType::BUTTON: {
			Button& button = element.element.button;
			if (elementIndex == 0) {
				yOffset -= getButtonHeight(button, textRenderer);
			}
			button.position = Vector2f { xPosition, yOffset };
			button.width = GlobalAppState.floatWidth * panel.percentageWidth - 2 * button.padding;
			button.update(textRenderer, event);
			yOffset -= getButtonHeight(button, textRenderer);
			break;
		}
		case UIElementType::TEXT_INPUT: {
			TextInput& textInput = element.element.textInput;
			if (elementIndex == 0) {
				yOffset -= textInput.bt.getBoundTextHeight(textRenderer);
			}
			textInput.bt.rect.x = xPosition;
			textInput.bt.rect.y = yOffset;
			textInput.bt.rect.w = GlobalAppState.floatWidth * panel.percentageWidth - 2 * textInput.bt.padding;
			textInput.update(textRenderer,  event);
			yOffset -= textInput.bt.getBoundTextHeight(textRenderer);
			break;
		}
		case UIElementType::LABEL: {
			Label& label = element.element.label;
			GLfloat btHeight = label.bt.getBoundTextHeight(textRenderer);
			if (elementIndex == 0) {
				yOffset -= btHeight;
			}

			label.bt.rect.x = xPosition;
			label.bt.rect.y = yOffset;
			label.bt.rect.w = GlobalAppState.floatWidth * panel.percentageWidth - 2 * label.bt.padding;
			label.bt.rect.h = btHeight;
			yOffset -= btHeight;
			break;
		}
		}

		yOffset -= spaceBetweenElements;
	}
}

void UIContext::render(const Shader& shader, const TextRenderer& textRenderer) const {
	if (!isActive) {
		return;
	}

	panel.render(shader);

	for (size_t elementIndex = 0; elementIndex < uiElements.numElements; elementIndex++) {
		UIElement& element = uiElements[elementIndex];
		switch (element.elementType) {
		case UIElementType::BUTTON: {
			element.element.button.render(shader, textRenderer);
			break;
		}
		case UIElementType::TEXT_INPUT: {
			element.element.textInput.render(shader, textRenderer);
			break;
		}
		case UIElementType::LABEL: {
			element.element.label.render(shader, textRenderer);
			break;
		}
		}
	}
}

void UIContext::free() {
	for (size_t eIdx = 0; eIdx < uiElements.numElements; eIdx++) {
		uiElements[eIdx].free();
	}

	uiElements.deallocate();
}