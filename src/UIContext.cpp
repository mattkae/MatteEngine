#include "UIContext.h"
#include "App.h"

void UIContext::init() {
	if (isClosable && !closeButton.label.isInited()) {
		closeButton.width = 16;
		closeButton.textColor = Vector4f{ 1.f, 1.f, 1.f, 1.f };
		closeButton.buttonColor = Vector4f{ 1.f, 1.f, 1.f, 1.f };
		closeButton.label = "X";
		closeButton.padding = 2.f;
		closeButton.buttonColor = 0xff3232;
		closeButton.hoverColor = 0xff6666;
	}
}

bool UIContext::isOpen() {
	return panel.panelState != PanelState_Hide;
}

void UIContext::update(float dtMs, const TextRenderer& textRenderer, UIEventProcessor* processor) {
	// @TODO: Could add some opening and closing animations around these
	if (shouldOpen) {
		shouldOpen = false;
		panel.show();
	} else if (shouldClose) {
		panel.hide();
		shouldClose = false;
	}

	if (panel.panelState == PanelState_Hide) {
		return;
	}

	panel.update(dtMs);


	// Y=0 is the bottom of the screen here.
	// To get to the start of the panel, we go to the top of the screen, and move
	// to the start of the panel (the panel's y-position plus its padding)
	GLfloat yOffset = GlobalApp.floatHeight - (panel.boundingRect.y + panel.padding);

	// X=0 is at the left of the screen
	GLfloat xPosition = panel.boundingRect.x + panel.padding;

	if (isClosable) {
		GLfloat elementHeight = getButtonHeight(closeButton, textRenderer);
		closeButton.position.x = xPosition + (panel.boundingRect.w - panel.padding - 16); // Right align
		closeButton.position.y = yOffset - elementHeight;
		closeButton.update(textRenderer, processor);
		yOffset -= (elementHeight + spaceBetweenElements);
	}
	
	// Organize thte elements vertically
	for (size_t elementIndex = 0; elementIndex < uiElements.numElements; elementIndex++) {
		GLfloat elementHeight = 0; // Basically how much to move the context by for each element
		UIElement& element = uiElements[elementIndex];
		switch (element.elementType) {
		case UIElementType::BUTTON: {
			Button& button = element.element.button;
			elementHeight = getButtonHeight(button, textRenderer);
			button.position = Vector2f { xPosition, yOffset - elementHeight};
			button.width = GlobalApp.floatWidth * panel.percentageWidth - 2 * button.padding;
			button.update(textRenderer, processor);
			break;
		}
		case UIElementType::TEXT_INPUT: {
			TextInput& textInput = element.element.textInput;
			elementHeight = textInput.bt.getBoundTextHeight(textRenderer);
			textInput.bt.rect.x = xPosition;
			textInput.bt.rect.y = yOffset - elementHeight;
			textInput.bt.rect.w = GlobalApp.floatWidth * panel.percentageWidth - 2 * textInput.bt.padding;
			textInput.update(textRenderer,  processor);
			break;
		}
		case UIElementType::LABEL: {
			Label& label = element.element.label;
			elementHeight = label.bt.getBoundTextHeight(textRenderer);
			label.bt.rect.x = xPosition;
			label.bt.rect.y = yOffset - elementHeight;
			label.bt.rect.w = GlobalApp.floatWidth * panel.percentageWidth - 2 * label.bt.padding;
			label.bt.rect.h = elementHeight;
			break;
		}
		}

		yOffset -= (elementHeight + spaceBetweenElements);
	}

	if (closeButton.isClicked) {
		shouldClose = true;
	}
}

void UIContext::render(const Shader& shader, const TextRenderer& textRenderer) const {
	if (panel.panelState == PanelState_Hide) {
		return;
	}

	panel.render(shader);

	if (isClosable) {
		closeButton.render(shader, textRenderer);
	}

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
	closeButton.free();

	for (size_t eIdx = 0; eIdx < uiElements.numElements; eIdx++) {
		uiElements[eIdx].free();
	}

	uiElements.deallocate();
}