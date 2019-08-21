#include "TextInput.h"
#include "Input.h"
#include "TextRenderer.h"
#include "Logger.h"
#include <GLFW/glfw3.h>

bool initializeTextInput(TextInput& input) {
	input.r = {
		input.position.x,
		input.position.y,
		input.dimensions.x,
		input.dimensions.y
	};
	input.r.backgroundColor = input.backgroundColor;
	input.r.borderColor = input.borderColor;
	input.focusToken = getNextFocusToken();
	initializeRenderableRectangle(input.r);
	return true;
}

void updateTextInput(TextInput& textInput) {
	updateRenderableRectangle(textInput.r);
	if (textInput.r.isFocused && textInput.r.isJustClicked) {
		setInputFocus(textInput.focusToken);
	} else if (getInputFocus() == textInput.focusToken && !textInput.r.isFocused) {
		resetInputFocus();
	}

	int key = getCurrentKeyDown(textInput.focusToken);
	int scancode = getCurrentScancode(textInput.focusToken);

	if (key > -1 && isKeyJustDown(key, textInput.focusToken)) {
		if (key == GLFW_KEY_BACKSPACE) {
			if (!textInput.str.empty()) {
				const size_t strSize = textInput.str.size();
				if (strSize == 1) {
					textInput.str = "";
				} else {
					textInput.str = textInput.str.substr(0, strSize - 1);
				}
			}
		} else {
			textInput.str += glfwGetKeyName(key, scancode);
		}
	}

}

void renderTextInput(const TextInput& textInput, const Shader& shader) {
	renderRenderableRectangle(textInput.r, shader);
	textInput.textRenderer->renderText(textInput.str, textInput.position, textInput.scale, textInput.textColor);
}