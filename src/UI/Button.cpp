#include "Button.h"
#include "../TextRenderer.h"

void initializeButton(Button& button, const TextRenderer& textRenderer) {
	button.mTextPosition = button.position + glm::vec2(2 * button.padding);
	GLfloat height = (textRenderer.getFontSize() + 2 * button.padding) * button.scale;
	button.rect.rect = { button.position.x, button.position.y, button.width, height };
}

void updateButton(Button& button) {
	updateRectangle(button.rect);
	if (button.rect.isJustClicked) {
		button.onClickHandler();
	}
}

void renderButton(const Button& button, const Shader& shader, const TextRenderer& textRenderer) {
	renderRectangle(button.rect.rect, shader,button. rect.isHovered ? button.hoverColor : button.buttonColor);
	textRenderer.renderText(button.label, button.mTextPosition, button.scale, button.textColor);
}