#include "BoundText.h"
#include "../TextRenderer.h"

void updateBoundText(BoundText& bt) {
	updateRectangle(bt.box);
}

void renderBoundText(const BoundText& bt, const Shader& shader, const TextRenderer& textRenderer, std::string text, glm::vec4 backgroundColor, glm::vec3 textColor, GLint scrollOffset) {
	renderRectangle(bt.box.rect, shader, backgroundColor);
	glm::vec2 textPosition = glm::vec2(bt.padding.x + bt.box.rect.x, bt.padding.y + bt.box.rect.y);
	GLfloat textWidthOffset = textRenderer.getStringWidth(text.substr(0, scrollOffset), bt.scale);

	if (textWidthOffset < bt.box.rect.w) {
		textWidthOffset = 0;
	} else {
		textWidthOffset = textWidthOffset - bt.box.rect.w;
	}

	glEnable(GL_SCISSOR_TEST);
	glScissor(static_cast<GLint>(bt.box.rect.x), static_cast<GLint>(bt.box.rect.y), static_cast<GLint>(bt.box.rect.w), static_cast<GLint>(bt.box.rect.h));
	textRenderer.renderText(text, textPosition, bt.scale, textColor, textWidthOffset);
	glDisable(GL_SCISSOR_TEST);
}

