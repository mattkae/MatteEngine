#include "TextInput.h"
#include "Input.h"
#include "TextRenderer.h"

bool initializeTextInput(TextInput& input) {
	return false;
}

void updateTextInput(TextInput& textInput) {
	updateRenderableRectangle(textInput.r);
}

void renderTextInput(const TextInput& textInput, const Shader& shader) {
	renderRenderableRectangle(textInput.r, shader);
	textInput.textRenderer->renderText(textInput.str, textInput.position, textInput.scale, textInput.textColor);
}