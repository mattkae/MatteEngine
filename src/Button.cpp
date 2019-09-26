#include "Button.h"
#include "TextRenderer.h"

void initializeButton(Button& button, const TextRenderer& textRenderer) {
	button.mTextPosition = button.position + glm::vec2(2 * button.padding);
	GLfloat height = (textRenderer.getFontSize() + 2 * button.padding) * button.scale;
	button.mRectangle.r = { button.position.x, button.position.y, button.width, height };
	initializeRenderableRectangle(button.mRectangle);
}

void updateButton(Button& button) {
	updateRenderableRectangle(button.mRectangle);
	if (button.mRectangle.isJustClicked) {
		button.onClickHandler();
	}
}

void renderButton(const Button& button, const Shader& shader, const TextRenderer& textRenderer) {
	renderRenderableRectangle(button.mRectangle, shader,button. mRectangle.isHovered ? button.hoverColor : button.buttonColor);
	textRenderer.renderText(button.label, button.mTextPosition, button.scale, button.textColor);
}