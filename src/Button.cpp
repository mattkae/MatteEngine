#include "Button.h"
#include "TextRenderer.h"
#include "Vector2f.h"

Rectangle getRectangle(const Button& button, const TextRenderer& textRenderer) {
	return { button.position.x, button.position.y, button.width, getButtonHeight(button, textRenderer) };
}

void Button::update(const TextRenderer& textRenderer, UIEventProcessor* processor) {
	boundingRect = getRectangle(*this, textRenderer);
	if (boundingRect.isClicked()) {
		if (!isClicked) {
			focusToken = grabFocus();
			isClicked = true;
			UIEvent activeEvent;
			activeEvent.type = eventType;
			activeEvent.data = &data;
			processor->processEvent(activeEvent);
		}
	} else if (isClicked && !boundingRect.isClicked(focusToken)) {
		isClicked = false;
		returnFocus(focusToken);
	}
}

void Button::render(const Shader& shader, const TextRenderer& textRenderer) const {
	Vector2f textPosition = position + Vector2f{2 * padding, 2 * padding};
	boundingRect.render(isClicked ? hoverColor : buttonColor);
	textRenderer.renderText(shader, label, textPosition, scale, textColor);
}

void Button::free() {
	label.free();
}