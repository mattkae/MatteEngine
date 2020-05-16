#include "UIContext.h"
#include "GlobalApplicationState.h"

void UIContext::update(const TextRenderer& textRenderer) {
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
		switch (element.type) {
		case UIElement::BUTTON: {
			Button& button = std::get<Button>(element.element);
			if (elementIndex == 0) {
				yOffset -= getButtonHeight(button, textRenderer);
			}
			button.position = Vector2f { xPosition, yOffset };
			button.width = GlobalAppState.floatWidth * panel.percentageWidth - 2 * button.padding;
			button.update(textRenderer);
			yOffset -= getButtonHeight(button, textRenderer);
			break;
		}
		case UIElement::TEXT_INPUT: {
			TextInput& textInput = std::get<TextInput>(element.element);
			if (elementIndex == 0) {
				yOffset -= textInput.bt.getBoundTextHeight(textRenderer);
			}
			textInput.bt.rect.x = xPosition;
			textInput.bt.rect.y = yOffset;
			textInput.bt.rect.w = GlobalAppState.floatWidth * panel.percentageWidth - 2 * textInput.bt.padding;
			textInput.update(textRenderer);
			yOffset -= textInput.bt.getBoundTextHeight(textRenderer);
			break;
		}
		case UIElement::LABEL: {
			Label& label = std::get<Label>(element.element);
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

void UIContext::render(const Shader& shader, const TextRenderer& textRenderer) {
	if (!isActive) {
		return;
	}

	panel.render(shader);

	for (size_t elementIndex = 0; elementIndex < uiElements.numElements; elementIndex++) {
		const UIElement& element = uiElements[elementIndex];
		switch (element.type) {
		case UIElement::BUTTON: {
			Button button = std::get<Button>(element.element);
			button.render(shader, textRenderer);
			break;
		}
		case UIElement::TEXT_INPUT: {
			TextInput textInput = std::get<TextInput>(element.element);
			textInput.render(shader, textRenderer);
			break;
		}
		case UIElement::LABEL: {
			Label label = std::get<Label>(element.element);
			label.render(shader, textRenderer);
			break;
		}
		}
	}
}

void UIContext::free() {
	uiElements.free();
}