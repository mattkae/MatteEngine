#include "Label.h"
#include "TextRenderer.h"

void Label::render(const Shader& shader, const TextRenderer& textRenderer) {
	bt.render(shader, textRenderer, text, backgroundColor, textColor);
}