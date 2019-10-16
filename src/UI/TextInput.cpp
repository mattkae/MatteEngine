#include "TextInput.h"
#include "../Input.h"
#include "../TextRenderer.h"
#include "../Logger.h"
#include <GLFW/glfw3.h>

Rectangle getRectangle(const TextInput& input, const TextRenderer& textRenderer) {
	return { input.bt.rect.x, input.bt.rect.y, input.bt.rect.w, getBoundTextHeight(input.bt, textRenderer) };
}

void updateTextInput(TextInput& textInput, const TextRenderer& textRenderer) {
	textInput.bt.rect = getRectangle(textInput, textRenderer);

	if (!textInput.isFocused) {
		if (isClicked(textInput.bt.rect)) {
			textInput.focusToken = grabFocus();
			textInput.isFocused = true;
		}
	} else if (textInput.isFocused) {
		if (isLeftClickDown() && !isClicked(textInput.bt.rect)) {
			returnFocus();
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
				textInput.str.erase(textInput.cursorPosition - 1, 1);
				textInput.cursorPosition--;
			}

			textInput.onChangeHandler(textInput.str);
			break;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT:
			break;
		default: 
			{
				// @TODO: Don't hardcode the key range that we support
				if (key > 128) {
					break;
				}
				char c = (char) key;
			
				if (!isKeyDown(GLFW_KEY_LEFT_SHIFT, textInput.focusToken)) {
					c = std::tolower(c);
				}

				if (textInput.cursorPosition == 0) {
					textInput.str = c + textInput.str;
				} else if (textInput.cursorPosition == textInput.str.size()) {
					textInput.str += c;
				} else {
					textInput.str = textInput.str.substr(0, textInput.cursorPosition) + c + textInput.str.substr(textInput.cursorPosition);
				}

				textInput.cursorPosition++;
				textInput.onChangeHandler(textInput.str);
			}
		}
	}
}

void renderTextInput(const TextInput& textInput, const Shader& shader, const TextRenderer& textRenderer) {
	renderBoundText(textInput.bt, shader, textRenderer, textInput.str, textInput.isFocused ? textInput.focusedBackgroundColor : textInput.backgroundColor, textInput.textColor, textInput.cursorPosition);
	
	if (textInput.isFocused) {
		GLfloat cursorOffset = textRenderer.getStringWidth(textInput.str.substr(0, textInput.cursorPosition), textInput.bt.scale);
		Rectangle cursorRect = { textInput.bt.rect.x + textInput.bt.padding + cursorOffset, textInput.bt.rect.y, 2, textRenderer.getFontSize() };
		renderRectangle(cursorRect, shader, glm::vec4(1, 0, 0, 1));
	}
}