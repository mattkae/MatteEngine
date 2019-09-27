#include "BoundText.h"
#include "TextRenderer.h";

void initializeBoundText(BoundText& bt) {
	initializeRenderableRectangle(bt.box);
}

void updateBoundText(BoundText& bt) {
	updateRenderableRectangle(bt.box);
}

void renderBoundText(const BoundText& bt, const Shader& shader, const TextRenderer& textRenderer, std::string text, glm::vec3 backgroundColor, glm::vec3 textColor, GLint scrollOffset) {
	// The bounding box can be of any height, but we will try and center the text vertically within it

	renderRenderableRectangle(bt.box, shader, backgroundColor);
	glm::vec2 textPosition = glm::vec2(bt.padding.x + bt.box.r.x, bt.padding.y + bt.box.r.y);
	GLint textWidthOffset = textRenderer.getStringWidth(text.substr(0, scrollOffset), bt.scale);

	if (textWidthOffset < bt.box.r.w) {
		textWidthOffset = 0;
	} else {
		textWidthOffset = textWidthOffset - bt.box.r.w;
	}

	glEnable(GL_SCISSOR_TEST);
	glScissor(bt.box.r.x, bt.box.r.y, bt.box.r.w, bt.box.r.h);
	textRenderer.renderText(text, textPosition, bt.scale, textColor, textWidthOffset);
	glDisable(GL_SCISSOR_TEST);
}