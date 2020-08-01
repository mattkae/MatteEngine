#include "Label.h"
#include "TextRenderer.h"

void Label::build(GLfloat padding, Vector4f bkColor, Vector4f txtColor, GLfloat scale) {
	bt.padding = 2.f;
	backgroundColor = Vector4f { 0.1f, 0.1f, 0.1f, 1.f };
	textColor = Vector4f { 1.f, 1.f, 1.f , 1.f };
	bt.scale = scale;
}

void Label::render(const Shader& shader, const TextRenderer& textRenderer) {
	bt.render(shader, textRenderer, text, backgroundColor, textColor);
}

void Label::free() {
	text.deallocate();
}