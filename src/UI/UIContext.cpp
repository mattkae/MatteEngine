#include "UIContext.h"
#include "../GlobalApplicationState.h"

void updateUIContext(UIContext& context, const TextRenderer& textRenderer) {
	setPanelPosition(context.panel);
	GLfloat yOffset = GlobalAppState.floatHeight - context.panel.boundingRect.y - context.panel.padding;
	GLfloat xPosition = context.panel.boundingRect.x + context.panel.padding;
	
	for (size_t elementIndex = 0; elementIndex < context.uiElements.numElements; elementIndex++) {
		UIElement& element = context.uiElements.elements[elementIndex];
		switch (element.type) {
		case UIElement::BUTTON: {
			Button& button = std::get<Button>(element.element);
			if (elementIndex == 0) {
				yOffset -= getButtonHeight(button, textRenderer);
			}
			button.position = glm::vec2(xPosition, yOffset);
			button.width = GlobalAppState.floatWidth * context.panel.percentageWidth - 2 * button.padding;
			updateButton(button, textRenderer);
			yOffset -= getButtonHeight(button, textRenderer);
			break;
		}
		case UIElement::TEXT_INPUT: {
			TextInput& textInput = std::get<TextInput>(element.element);
			if (elementIndex == 0) {
				yOffset -= getTextInputHeight(textInput, textRenderer);
			}
			textInput.bt.rect.x = xPosition;
			textInput.bt.rect.y = yOffset;
			textInput.bt.rect.w = GlobalAppState.floatWidth * context.panel.percentageWidth - 2 * textInput.bt.padding;
			updateTextInput(textInput, textRenderer);
			yOffset -= getTextInputHeight(textInput, textRenderer);
			break;
		}
		}

		yOffset -= context.spaceBetweenElements;
	}

	for (size_t contextIndex = 0; contextIndex < context.dependentContexts.numElements; contextIndex++) {
		UIContext& dependentContext = context.dependentContexts.elements[contextIndex];
		if (dependentContext.isActive) {
			updateUIContext(dependentContext, textRenderer);
		}
	}
}

void renderUIContext(const UIContext& context, const Shader& shader, const TextRenderer& textRenderer) {
	renderPanel(context.panel, shader);

	for (size_t elementIndex = 0; elementIndex < context.uiElements.numElements; elementIndex++) {
		const UIElement& element = context.uiElements.elements[elementIndex];
		switch (element.type) {
		case UIElement::BUTTON:
			renderButton(std::get<Button>(element.element), shader, textRenderer);
			break;
		case UIElement::TEXT_INPUT:
			renderTextInput(std::get<TextInput>(element.element), shader, textRenderer);
			break;
		}
	}

	for (size_t contextIndex = 0; contextIndex < context.dependentContexts.numElements; contextIndex++) {
		const UIContext& dependentContext = context.dependentContexts.elements[contextIndex];
		if (dependentContext.isActive) {
			renderUIContext(dependentContext, shader, textRenderer);
		}
	}
}