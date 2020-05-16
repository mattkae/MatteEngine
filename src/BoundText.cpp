#include "BoundText.h"
#include "TextRenderer.h"
#include "Vector2f.h"

void BoundText::render(const Shader& shader, const TextRenderer& textRenderer, 
	std::string text, const Vector4f& backgroundColor, const Vector4f& textColor, GLint scrollOffset) {
	rect.render(shader, backgroundColor);
	Vector2f textPosition = Vector2f { padding + rect.x, padding + rect.y };
	GLfloat textWidthOffset = textRenderer.getStringWidth(text.substr(0, scrollOffset), scale);

	if (textWidthOffset < rect.w) {
		textWidthOffset = 0;
	} else {
		textWidthOffset = textWidthOffset - rect.w;
	}

	glEnable(GL_SCISSOR_TEST);
	glScissor(static_cast<GLint>(rect.x), static_cast<GLint>(rect.y), static_cast<GLint>(rect.w), static_cast<GLint>(rect.h));
	textRenderer.renderText(shader, text, textPosition, scale, textColor, textWidthOffset);
	glDisable(GL_SCISSOR_TEST);
}

