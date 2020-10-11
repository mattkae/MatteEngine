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
	return panel.panelState != PanelState_Hide && panel.panelState != PanelState_Closing;
}

void UIContext::update(float dtMs, const TextRenderer& textRenderer, UIEventProcessor* processor) {
	// @TODO: Could add some opening and closing animations around these
	if (shouldOpen && !isOpen()) {
		shouldOpen = false;
		panel.show();
	} else if (shouldClose) {
		panel.hide();
		shouldClose = false;
	}

	panel.update(dtMs);
	if (panel.panelState == PanelState_Hide) {
		return;
	}

	// Y=0 is the bottom of the screen here.
	// To get to the start of the panel, we go to the top of the screen, and move
	// to the start of the panel (the panel's y-position plus its padding)
	GLfloat yStart = GlobalApp.floatHeight - (panel.boundingRect.y + panel.padding);
	GLfloat yOffset = yStart;

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

	if (textureDebugger.show) {
		textureDebugger.rectangle = {
			xPosition, 
			yOffset - 150.f,
			panel.boundingRect.w - panel.padding,
			150.f
		};
		yOffset -= 150.f;
	}

	const GLfloat renderedHeight = fabs(yOffset);
	if (renderedHeight > panel.boundingRect.h) {
		// Add the scrollbar
		GLfloat scrollbarHeight = 12.f;
		Rectangle scrollbar = {
			xPosition + panel.boundingRect.w - 2.f, // Scroll bar is 2 units wide
			yStart,
			2,
			scrollbarHeight
		};
		scrollbar.render(Vector4f { 0, 1, 0, 1 }, Vector4f { 1, 0, 0, 1 }, 1.f);
	}

	if (closeButton.isClicked) {
		shouldClose = true;
	}
}

void UIContext::render(const Shader& shader, const TextRenderer& textRenderer) const {
	if (panel.panelState == PanelState_Hide) {
		return;
	}

	glEnable(GL_SCISSOR_TEST);
	glScissor(static_cast<int>(panel.boundingRect.x - panel.padding),
		static_cast<int>(panel.boundingRect.y + panel.padding),
		static_cast<int>(panel.boundingRect.w + 2 * panel.padding),
		static_cast<int>(panel.boundingRect.h + 2 * panel.padding));

	panel.render(shader);

	if (isClosable) {
		closeButton.render(shader, textRenderer);
	}

	for (size_t elementIndex = 0; elementIndex < uiElements.numElements; elementIndex++) {
		// Bound text also calls GL_SCISSOR_TEST to clip text in the rectangle
		glEnable(GL_SCISSOR_TEST);
		glScissor(static_cast<int>(panel.boundingRect.x - panel.padding),
		static_cast<int>(panel.boundingRect.y + panel.padding),
		static_cast<int>(panel.boundingRect.w + 2 * panel.padding),
		static_cast<int>(panel.boundingRect.h + 2 * panel.padding));


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

	if (textureDebugger.show) {
		textureDebugger.render();
	}

	glDisable(GL_SCISSOR_TEST);
}

void UIContext::free() {
	closeButton.free();

	for (size_t eIdx = 0; eIdx < uiElements.numElements; eIdx++) {
		uiElements[eIdx].free();
	}

	uiElements.deallocate();
}

void TextureDebugView::render() const {
	if (!show) {
		return;
	}

	rectangle.renderTexture(texture, { 0.1f, 0.1f, 0.1f, 1.0 }, 2.f);
}
