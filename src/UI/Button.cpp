#include "Button.h"
#include "../TextRenderer.h"
#include "../Primitives/Rectangle.h"

Rectangle getRectangle(const Button& button, const TextRenderer& textRenderer) {
	return { button.position.x, button.position.y, button.width, getButtonHeight(button, textRenderer) };
}

void updateButton(Button& button, const TextRenderer& textRenderer) {
	Rectangle rect = getRectangle(button, textRenderer);
	if (button.isClicked != nullptr) {
		if (!*button.isClicked) {
			if (isClicked(rect)) {
				*button.isClicked = true;
			}
		} else if (!isClicked(rect)) {
			*button.isClicked = false;
		}
	}
}

void renderButton(const Button& button, const Shader& shader, const TextRenderer& textRenderer) {
	glm::vec2 textPosition = button.position + glm::vec2(2 * button.padding);
	Rectangle rect = getRectangle(button, textRenderer);
	renderRectangle(rect, shader, *button.isClicked ? button.hoverColor : button.buttonColor);
	textRenderer.renderText(shader, button.label, textPosition, button.scale, button.textColor);
}