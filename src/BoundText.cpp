#include "BoundText.h"
#include "TextRenderer.h"
#include "Vector2f.h"

void renderBoundText(const BoundText& bt, const Shader& shader, const TextRenderer& textRenderer, 
	std::string text, const Vector4f& backgroundColor, const Vector4f& textColor, GLint scrollOffset) {
	renderRectangle(bt.rect, shader, backgroundColor);
	Vector2f textPosition = Vector2f { bt.padding + bt.rect.x, bt.padding + bt.rect.y };
	GLfloat textWidthOffset = textRenderer.getStringWidth(text.substr(0, scrollOffset), bt.scale);

	if (textWidthOffset < bt.rect.w) {
		textWidthOffset = 0;
	} else {
		textWidthOffset = textWidthOffset - bt.rect.w;
	}

	glEnable(GL_SCISSOR_TEST);
	glScissor(static_cast<GLint>(bt.rect.x), static_cast<GLint>(bt.rect.y), static_cast<GLint>(bt.rect.w), static_cast<GLint>(bt.rect.h));
	textRenderer.renderText(shader, text, textPosition, bt.scale, textColor, textWidthOffset);
	glDisable(GL_SCISSOR_TEST);
}

