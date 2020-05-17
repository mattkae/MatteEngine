#include "Button.h"
#include "TextRenderer.h"
#include "Vector2f.h"
#include "UI.h"

Rectangle getRectangle(const Button& button, const TextRenderer& textRenderer) {
	return { button.position.x, button.position.y, button.width, getButtonHeight(button, textRenderer) };
}

void Button::update(const TextRenderer& textRenderer) {
	boundingRect = getRectangle(*this, textRenderer);
	if (boundingRect.isClicked()) {
		if (!isClicked) {
			isClicked = true;
			(uiRef->*onClick)(data);
		}
	} else if (isClicked) {
		isClicked = false;
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