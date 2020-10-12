#include "UIContext.h"
#include "App.h"
#include "MathHelper.h"

const float SCROLLBAR_WIDTH = 8;
const float CLOSEBUTTON_WIDTH = 16;
const float TEXTURE_DEBUGGER_HEIGHT = 150.f;

void UIContext::init() {
	if (isClosable && !closeButton.label.isInited()) {
		closeButton.width = CLOSEBUTTON_WIDTH;
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
	GLfloat panelTop = GlobalApp.floatHeight - (panel.boundingRect.y + panel.padding);	// Top of the panel
	GLfloat currYPosition = panelTop + scrollAmount;									// Current render position of the next element

	// X=0 is at the left of the screen
	GLfloat xPosition = panel.boundingRect.x + panel.padding;
	GLfloat xRenderSpace =  GlobalApp.floatWidth * panel.percentageWidth - 2;
	if (scrollbar.show) {
		xRenderSpace -= SCROLLBAR_WIDTH;
	}

	if (isClosable) {
		GLfloat elementHeight = getButtonHeight(closeButton, textRenderer);
		closeButton.position.x = xPosition + (xRenderSpace - CLOSEBUTTON_WIDTH); // Right align
		closeButton.position.y = currYPosition - elementHeight;
		closeButton.update(textRenderer, processor);
		currYPosition -= (elementHeight + spaceBetweenElements);
	}
	
	// Organize thte elements vertically
	for (size_t elementIndex = 0; elementIndex < uiElements.numElements; elementIndex++) {
		GLfloat elementHeight = 0; // Basically how much to move the context by for each element
		UIElement& element = uiElements[elementIndex];
		switch (element.elementType) {
		case UIElementType::BUTTON: {
			Button& button = element.element.button;
			elementHeight = getButtonHeight(button, textRenderer);
			button.position = Vector2f { xPosition, currYPosition - elementHeight};
			button.width = xRenderSpace - 2 * button.padding;
			button.update(textRenderer, processor);
			break;
		}
		case UIElementType::TEXT_INPUT: {
			TextInput& textInput = element.element.textInput;
			elementHeight = textInput.bt.getBoundTextHeight(textRenderer);
			textInput.bt.rect.x = xPosition;
			textInput.bt.rect.y = currYPosition - elementHeight;
			textInput.bt.rect.w = xRenderSpace - 2 * textInput.bt.padding;
			textInput.update(textRenderer,  processor);
			break;
		}
		case UIElementType::LABEL: {
			Label& label = element.element.label;
			elementHeight = label.bt.getBoundTextHeight(textRenderer);
			label.bt.rect.x = xPosition;
			label.bt.rect.y = currYPosition - elementHeight;
			label.bt.rect.w = xRenderSpace - 2 * label.bt.padding;
			label.bt.rect.h = elementHeight;
			break;
		}
		}

		currYPosition -= (elementHeight + spaceBetweenElements);
	}

	if (textureDebugger.show) {
		textureDebugger.rectangle = {
			xPosition, 
			currYPosition - TEXTURE_DEBUGGER_HEIGHT,
			panel.boundingRect.w - panel.padding,
			TEXTURE_DEBUGGER_HEIGHT
		};
		currYPosition -= TEXTURE_DEBUGGER_HEIGHT;
	}

	// If we're outside of the height, let's add a scrollbar
	const GLfloat renderedHeight = fabs(currYPosition - scrollAmount - panelTop);
	scrollbar.show = renderedHeight > panel.boundingRect.h;
	if (scrollbar.show) {
		// Calculate the scrollbar height and current position
		// Resource: https://stackoverflow.com/questions/16366795/how-to-calculate-the-size-of-scroll-bar-thumb/22710156
		const GLfloat scrollbarArea = panel.boundingRect.h;
		const GLfloat scrollbarHeight = scrollbarArea * (panel.boundingRect.h / renderedHeight);
		scrollbar.rectangle = {
			xPosition + panel.boundingRect.w - panel.padding - SCROLLBAR_WIDTH, // Right aligned
			panelTop - scrollbarHeight - scrollbar.yOffset,
			SCROLLBAR_WIDTH,
			scrollbarHeight
		};
		
		if (scrollbar.isClicked) {
			if (!isLeftClickDown(scrollbar.inputToken)) {
				scrollbar.isClicked = false;
				returnFocus(scrollbar.inputToken);
			} else {
				// Calculate how much we've moved
				const GLfloat scrollbarTop = panelTop - scrollbarHeight;
				const GLfloat scrollbarBottom = panelTop - panel.boundingRect.h;
				const GLfloat scrollSpaceY = scrollbarTop - scrollbarBottom;
				const Vector2f currentPosition = getCursorPosition();
				const GLfloat scrollbarMoveY = scrollbar.originalClickSpot.y - currentPosition.y;
				scrollbar.yOffset = CLAMP(scrollbar.yOffset + scrollbarMoveY, scrollSpaceY, 0.f);
				scrollbar.rectangle.y -= scrollbarMoveY;
				scrollbar.originalClickSpot = currentPosition;

				// Force the scroll bar to stay in the panel
				if (scrollbar.rectangle.y >= scrollbarTop) {
					scrollbar.rectangle.y = scrollbarTop;
				} else if (scrollbar.rectangle.y <= scrollbarBottom) {
					scrollbar.rectangle.y = scrollbarBottom;
				}

				const GLfloat scrollPercentage = CLAMP((scrollbar.yOffset) / scrollSpaceY, 1.0f, 0.f);
				scrollAmount = scrollPercentage * panel.boundingRect.h;
			}
		} else if (scrollbar.rectangle.isClicked()) {
			scrollbar.isClicked = true;
			scrollbar.originalClickSpot = getCursorPosition();
			scrollbar.inputToken = grabFocus();
		}
	}

	if (closeButton.isClicked) {
		shouldClose = true;
	}
}

inline void clipPanel(const Panel& panel) {
	glScissor(static_cast<int>(panel.boundingRect.x),
		static_cast<int>(panel.boundingRect.y - panel.padding),
		static_cast<int>(panel.boundingRect.w),
		static_cast<int>(panel.boundingRect.h - 2 * panel.padding));
}

void UIContext::render(const Shader& shader, const TextRenderer& textRenderer) const {
	if (panel.panelState == PanelState_Hide) {
		return;
	}

	glEnable(GL_SCISSOR_TEST);
	clipPanel(panel);

	panel.render(shader);

	if (isClosable) {
		closeButton.render(shader, textRenderer);
	}

	for (size_t elementIndex = 0; elementIndex < uiElements.numElements; elementIndex++) {
		// Bound text also calls GL_SCISSOR_TEST to clip text in the rectangle
		glEnable(GL_SCISSOR_TEST);
		clipPanel(panel);

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

	if (scrollbar.show) {
		if (scrollbar.isClicked) {
			scrollbar.rectangle.render(Vector4f { 0, 1, 0, 1 });
		} else {
			scrollbar.rectangle.render(Vector4f { 1, 0, 0, 1 });
		}
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
