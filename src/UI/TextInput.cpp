#include "TextInput.h"
#include "../Input.h"
#include "../TextRenderer.h"
#include "../Logger.h"
#include <GLFW/glfw3.h>

Rectangle getRectangle(const TextInput& input, const TextRenderer& textRenderer) {
	return { input.bt.rect.x, input.bt.rect.y, input.bt.rect.w, getBoundTextHeight(input.bt, textRenderer) };
}

inline void onStrChange(TextInput& textInput, std::string v) {
	switch (textInput.inputType) {
	case TextInputType::TEXT: {
		*textInput.value.sVal = v;
		break;
	}
	case TextInputType::INT: {
		*textInput.value.iVal = std::stoi(v);
		break;
	}
	case TextInputType::FLOAT: {
		*textInput.value.fVal = std::stof(v);
		break;
	}
	default:
		// @TODO: Log error
		break;
	}
}

inline void setInternalRepresentation(TextInput& textInput) {
	if (textInput.representation.size() != 0) {
		return;
	}

	switch (textInput.inputType) {
	case TextInputType::TEXT: 
		textInput.representation = *textInput.value.sVal;
		break;
	case TextInputType::INT:
		textInput.representation = std::to_string(*textInput.value.iVal);
		break;
	case TextInputType::FLOAT: 
		textInput.representation = std::to_string(*textInput.value.fVal);
		break;
	default:
		// @TODO: Log error
		break;
	}
}

void updateTextInput(TextInput& textInput, const TextRenderer& textRenderer) {
	setInternalRepresentation(textInput);
	textInput.bt.rect = getRectangle(textInput, textRenderer);

	if (!textInput.isFocused) {
		if (isClicked(textInput.bt.rect)) {
			textInput.focusToken = grabFocus();
			textInput.isFocused = true;
		}
	} else if (textInput.isFocused) {
		if (isLeftClickDown() && !isClicked(textInput.bt.rect)) {
			returnFocus(textInput.focusToken);
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
		case GLFW_KEY_RIGHT: {
			textInput.cursorPosition = textInput.cursorPosition == textInput.representation.size() ? textInput.representation.size() : textInput.cursorPosition + 1;
			break;
		}
		case GLFW_KEY_BACKSPACE:
			if (!textInput.representation.empty() && textInput.cursorPosition != 0) {
				textInput.representation.erase(textInput.cursorPosition - 1, 1);
				textInput.cursorPosition--;
			}

			onStrChange(textInput, textInput.representation);
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
					textInput.representation = c + textInput.representation;
				} else if (textInput.cursorPosition == textInput.representation.size()) {
					textInput.representation += c;
				} else {
					textInput.representation = textInput.representation.substr(0, textInput.cursorPosition) + c + textInput.representation.substr(textInput.cursorPosition);
				}

				textInput.cursorPosition++;
				onStrChange(textInput, textInput.representation);
			}
		}
	}
}

void renderTextInput(const TextInput& textInput, const Shader& shader, const TextRenderer& textRenderer) {
	renderBoundText(textInput.bt, shader, textRenderer, textInput.representation, textInput.isFocused ? textInput.focusedBackgroundColor : textInput.backgroundColor, textInput.textColor, textInput.cursorPosition);
	
	if (textInput.isFocused) {
		GLfloat cursorOffset = textRenderer.getStringWidth(textInput.representation.substr(0, textInput.cursorPosition), textInput.bt.scale);
		Rectangle cursorRect = { textInput.bt.rect.x + textInput.bt.padding + cursorOffset, textInput.bt.rect.y, 2, textRenderer.getFontSize() };
		renderRectangle(cursorRect, shader, glm::vec4(1, 0, 0, 1));
	}
}