#include "Button.h";
#include "TextRenderer.h";
#include "Shader.h"
#include "Input.h"

Button::Button() {

}

void Button::initialize() {
	glGenVertexArrays(1, &mVao);
	glGenBuffers(1, &mVbo);
	glGenBuffers(1, &mEbo);

	mTextPosition = position + glm::vec2(2 * padding);

	GLfloat width = textRenderer->getStringWidth(label, scale) + 2 * padding;
	GLfloat height = (textRenderer->getFontSize() + 2 * padding) * scale;

	GLfloat xStart = position.x;
	GLfloat xEnd = position.x + width;
	GLfloat yStart = position.y;
	GLfloat yEnd = position.y + height;

	GLfloat vertices[8] = {
		xEnd, yEnd,
		xEnd, yStart,
		xStart, yStart,
		xStart, yEnd
	};

	GLint indicies[6] = {
		0, 1, 3, 1, 2, 3
	};

	glBindVertexArray(mVao);
	glBindBuffer(GL_ARRAY_BUFFER, mVbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indicies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (GLvoid*) 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	mBoundingRectangle = { xStart, yStart, width, height };
}

void Button::update() {
	const Input& input = Input::getInstance();
	DoublePoint point = input.getCursorPosition();
	mIsHovered = doesIntersectBox(mBoundingRectangle, point);

	if (mIsHovered && !mIsClicked && input.isLeftClickDown()) {
		this->onClickHandler();
		mIsClicked = true;
	} else if (!input.isLeftClickDown() && mIsClicked) {
		mIsClicked = false;
	}
}

void Button::render(const Shader& shader) const {
	// Render the box
	shader.setVec3("uColor", mIsHovered ? hoverColor : buttonColor);

	glBindVertexArray(mVao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Render the label
	textRenderer->renderText(label, mTextPosition, scale, textColor);
}