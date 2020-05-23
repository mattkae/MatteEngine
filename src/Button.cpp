#include "Button.h"
#include "TextRenderer.h"
#include "Vector2f.h"
#include "SceneUIController.h"

Rectangle getRectangle(const Button& button, const TextRenderer& textRenderer) {
	return { button.position.x, button.position.y, button.width, getButtonHeight(button, textRenderer) };
}

void Button::update(const TextRenderer& textRenderer, UIEvent& event) {
	boundingRect = getRectangle(*this, textRenderer);
	if (boundingRect.isClicked()) {
		if (!isClicked) {
			focusToken = grabFocus();
			isClicked = true;
			event.type = eventType;
			event.data = &data;
		}
	} else if (isClicked) {
		isClicked = false;
		returnFocus(focusToken);
	}
}

void Button::render(const Shader& shader, const TextRenderer& textRenderer) {
	Vector2f textPosition = position + Vector2f{2 * padding, 2 * padding};
	boundingRect.render(shader, isClicked ? hoverColor : buttonColor);
	textRenderer.renderText(shader, label, textPosition, scale, textColor);
}

void Button::free() {
	label.deallocate();
}