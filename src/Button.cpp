#include "Button.h"
#include "TextRenderer.h"

Button::Button() {

}

void Button::initialize() {
	mTextPosition = position + glm::vec2(2 * padding);
	GLfloat height = (textRenderer->getFontSize() + 2 * padding) * scale;
	mRectangle.r = { position.x, position.y, width, height };
	initializeRenderableRectangle(mRectangle);
}

void Button::update() {
	updateRenderableRectangle(mRectangle);
	if (mRectangle.isJustClicked) {
		this->onClickHandler();
	}
}

void Button::render(const Shader& shader) const {
	renderRenderableRectangle(mRectangle, shader, mRectangle.isHovered ? hoverColor : buttonColor);
	textRenderer->renderText(label, mTextPosition, scale, textColor);
}