#include "Label.h"
#include "TextRenderer.h"

void renderLabel(const Label& label, const Shader& shader, const TextRenderer& textRenderer) {
	renderBoundText(label.bt, shader, textRenderer, label.text, label.backgroundColor, label.textColor);
}