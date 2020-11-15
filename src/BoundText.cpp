#include "BoundText.h"
#include "TextRenderer.h"
#include "Vector2f.h"

void BoundText::render(const Shader& shader, const TextRenderer& textRenderer, 
	String& text, const Vector4f& backgroundColor, const Vector4f& textColor, GLint scrollOffset) {
	rect.render(backgroundColor);
	Vector2f textPosition = Vector2f { padding + rect.x, padding + rect.y };
	GLfloat textWidthOffset = textRenderer.getStringWidth(text.substring(0, scrollOffset), scale);

	if (textWidthOffset < rect.w) {
		textWidthOffset = 0;
	} else {
		textWidthOffset = textWidthOffset - rect.w;
	}

	int prevScissor[4];
	bool hasScissor = glIsEnabled(GL_SCISSOR_TEST);
    glGetIntegerv(GL_SCISSOR_BOX, prevScissor);

	Rectangle scissorRect;
	if (!hasScissor){
		glEnable(GL_SCISSOR_TEST);
		scissorRect = rect;
	} else {
		Rectangle firstScissor;
		firstScissor.x = static_cast<float>(prevScissor[0]);
		firstScissor.y = static_cast<float>(prevScissor[1]);
		firstScissor.w = static_cast<float>(prevScissor[2]);
		firstScissor.h = static_cast<float>(prevScissor[3]);

		if (!firstScissor.isOverlapping(&rect)) {
			scissorRect = { 0, 0, 0, 0 };
		} else {
			scissorRect = firstScissor.getOverlap(&rect);
		}
	}

	glScissor(static_cast<GLint>(scissorRect.x), static_cast<GLint>(scissorRect.y), static_cast<GLint>(scissorRect.w), static_cast<GLint>(scissorRect.h));
	textRenderer.renderText(shader, text, textPosition, scale, textColor, textWidthOffset);

	if (!hasScissor){
		glDisable(GL_SCISSOR_TEST);
	}
}

void BoundText::render(const Shader& shader, const TextRenderer& textRenderer, 
	StringBuilder& sb, const Vector4f& backgroundColor, const Vector4f& textColor, GLint scrollOffset) {
	rect.render(backgroundColor);
	Vector2f textPosition = Vector2f { padding + rect.x, padding + rect.y };

	GLfloat textWidthOffset  = 0;
	for (int cIdx = 0; cIdx < scrollOffset; cIdx++) {
		textWidthOffset += textRenderer.getCharWidth(sb.getCharAtIdx(cIdx), scale);
	}

	if (textWidthOffset < rect.w) {
		textWidthOffset = 0;
	} else {
		textWidthOffset = textWidthOffset - rect.w;
	}

	int prevScissor[4];
	bool hasScissor = glIsEnabled(GL_SCISSOR_TEST);
    glGetIntegerv(GL_SCISSOR_BOX, prevScissor);

	Rectangle scissorRect;
	if (!hasScissor){
		glEnable(GL_SCISSOR_TEST);
		scissorRect = rect;
	} else {
		Rectangle firstScissor;
		firstScissor.x = static_cast<float>(prevScissor[0]);
		firstScissor.y = static_cast<float>(prevScissor[1]);
		firstScissor.w = static_cast<float>(prevScissor[2]);
		firstScissor.h = static_cast<float>(prevScissor[3]);

		if (!firstScissor.isOverlapping(&rect)) {
			scissorRect = { 0, 0, 0, 0 };
		} else {
			scissorRect = firstScissor.getOverlap(&rect);
		}
	}

	glScissor(static_cast<GLint>(scissorRect.x), static_cast<GLint>(scissorRect.y), static_cast<GLint>(scissorRect.w), static_cast<GLint>(scissorRect.h));
	textRenderer.renderBuilder(shader, sb, textPosition, scale, textColor, textWidthOffset);

	if (!hasScissor){
		glDisable(GL_SCISSOR_TEST);
	}
}