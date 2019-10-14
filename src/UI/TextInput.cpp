#include "TextInput.h"
#include "../Input.h"
#include "../TextRenderer.h"
#include "../Logger.h"
#include <GLFW/glfw3.h>

bool initializeTextInput(TextInput& input) {
	input.focusToken = getNextFocusToken();
	input.cursorPosition = input.str.size();
	input.currentBackgroundColor = input.backgroundColor;
	return true;
}

Rectangle getRectangle(const TextInput& input, const TextRenderer& textRenderer) {
	return { input.bt.rect.x, input.bt.rect.y, input.bt.rect.w, getTextInputHeight(input, textRenderer) };
}


void updateTextInput(TextInput& textInput, const TextRenderer& textRenderer) {
	textInput.bt.rect = getRectangle(textInput, textRenderer);

	if (!textInput.isFocused) {
		if (isClicked(textInput.bt.rect)) {
			setInputFocus(textInput.focusToken);
			textInput.currentBackgroundColor = textInput.focusedBackgroundColor;
			textInput.isFocused = true;
		}
	} else if (textInput.isFocused) {
		if (isLeftClickDown() && !isClicked(textInput.bt.rect)) {
			resetInputFocus();
			textInput.currentBackgroundColor = textInput.backgroundColor;
			textInput.isFocused = false;
		}
	}

	int key = getCurrentKeyDown(textInput.focusToken);
	int scancode = getCurrentScancode(textInput.focusToken);

	if (key > -1 && isKeyJustDown(key, textInput.focusToken)) {
		switch (key) {
		case GLFW_KEY_LEFT:
			textInput.cursorPosition = textInput.cursorPosition == 0 ? 0 : textInput.cursorPosition - 1;
			break;
		case GLFW_KEY_RIGHT:
			textInput.cursorPosition = textInput.cursorPosition == textInput.str.size() ? textInput.str.size() : textInput.cursorPosition + 1;
			break;
		case GLFW_KEY_BACKSPACE:
			if (!textInput.str.empty() && textInput.cursorPosition != 0) {
				textInput.str.erase(textInput.cursorPosition - 1, textInput.cursorPosition);
				textInput.cursorPosition--;
			}

			textInput.onChangeHandler(textInput.str);
			break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			break;
		default: 
			{
				char c = (char) key;
			
				if (!isKeyDown(GLFW_KEY_LEFT_SHIFT, textInput.focusToken)) {
					c = std::tolower(c);
				}

				if (textInput.cursorPosition == textInput.str.size()) {
					textInput.str += c;
				} else {
					textInput.str.insert(textInput.cursorPosition, &c);
				}

				textInput.cursorPosition++;
				textInput.onChangeHandler(textInput.str);
			}
		}
	}
}

void renderTextInput(const TextInput& textInput, const Shader& shader, const TextRenderer& textRenderer) {
	renderBoundText(textInput.bt, shader, textRenderer, textInput.str, textInput.currentBackgroundColor, textInput.textColor, textInput.cursorPosition);
}