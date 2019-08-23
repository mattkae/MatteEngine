#include "Button.h"
#include "TextRenderer.h"
#include "Shader.h"

Button::Button() {

}

void Button::initialize() {
	mTextPosition = position + glm::vec2(2 * padding);
	GLfloat height = (textRenderer->getFontSize() + 2 * padding) * scale;
	mRectangle.backgroundColor = buttonColor;
	mRectangle.r = { position.x, position.y, width, height };
	initializeRenderableRectangle(mRectangle);
}

void Button::update() {
	updateRenderableRectangle(mRectangle);
	if (mRectangle.isJustClicked) {
		this->onClickHandler();
	}

	mRectangle.backgroundColor = mRectangle.isHovered ? hoverColor : buttonColor;
}

void Button::render(const Shader& shader) const {
	renderRenderableRectangle(mRectangle, shader);
	textRenderer->renderText(label, mTextPosition, scale, textColor);
}