#include "Button.h"
#include "TextRenderer.h"
#include "Rectangle.h"
#include "Vector2f.h"

Rectangle getRectangle(const Button& button, const TextRenderer& textRenderer) {
	return { button.position.x, button.position.y, button.width, getButtonHeight(button, textRenderer) };
}

void updateButton(Button& button, const TextRenderer& textRenderer) {
	Rectangle rect = getRectangle(button, textRenderer);
	if (button.isClicked != nullptr) {
		if (!(*button.isClicked)) {
			if (isClicked(rect)) {
				*button.isClicked = true;
			}
		}
	}
}

void renderButton(const Button& button, const Shader& shader, const TextRenderer& textRenderer) {
	Vector2f textPosition = button.position + Vector2f{2 * button.padding, 2 * button.padding};
	Rectangle rect = getRectangle(button, textRenderer);
	renderRectangle(rect, shader, *button.isClicked ? button.hoverColor : button.buttonColor);
	textRenderer.renderText(shader, button.label, textPosition, button.scale, button.textColor);
}