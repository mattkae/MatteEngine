#include "TextInput.h"
#include "Input.h"
#include "TextRenderer.h"
#include "Logger.h"
#include <GLFW/glfw3.h>

Rectangle getRectangle(const TextInput& input, const TextRenderer& textRenderer) {
	GLfloat height = input.bt.getBoundTextHeight(textRenderer);
	return { input.bt.rect.x, input.bt.rect.y, input.bt.rect.w, height };
}

inline void setInternalRepresentation(TextInput& textInput, bool force) {
	// @TODO: Add back in check and think of a workaround if this becomes to slow for some reason
	//if (textInput.representation.size() != 0 && !force) {
	//	return;
	//}

	switch (textInput.inputType) {
	case TextInputType::TEXT: 
		//textInput.representation = *textInput.value.sVal;
		break;
	case TextInputType::INT:
		if (textInput.lastValue.iVal != *textInput.value.iVal) {
			textInput.lastValue.iVal = *textInput.value.iVal;
			textInput.representation.fromInteger(*textInput.value.iVal);
		} else {
			return;
		}
		break;
	case TextInputType::FLOAT: 
		if (textInput.lastValue.fVal != *textInput.value.fVal) {
			textInput.lastValue.fVal = *textInput.value.fVal;
			textInput.representation.fromFloat(*textInput.value.fVal);
		} else {
			return;
		}
		break;
	default:
		// @TODO: Log error
		break;
	}

	if (textInput.cursorPosition > textInput.representation.length) {
		textInput.cursorPosition = textInput.representation.length;
	}
}

inline void onStrChange(TextInput& textInput, String v) {
	switch (textInput.inputType) {
	case TextInputType::TEXT: {
		//*textInput.value.sVal = v;
		break;
	}
	case TextInputType::INT: {
		*textInput.value.iVal = v.toInteger();
		break;
	}
	case TextInputType::FLOAT: {
		*textInput.value.fVal = v.toFloat();
		break;
	}
	default:
		// @TODO: Log error
		break;
	}

	setInternalRepresentation(textInput, true);
}

void TextInput::update(const TextRenderer& textRenderer) {
	setInternalRepresentation(*this, false);
	bt.rect = getRectangle(*this, textRenderer);

	if (!isFocused) {
		if (bt.rect.isClicked()) {
			focusToken = grabFocus();
			isFocused = true;
		}
	} else if (isFocused) {
		if (isLeftClickDown() && !bt.rect.isClicked()) {
			returnFocus(focusToken);
			isFocused = false;
			onStrChange(*this, representation);
		}
	}

	int key = getCurrentKeyDown(focusToken);
	int scancode = getCurrentScancode(focusToken);

	if (key > -1 && isKeyJustDown(key, focusToken)) {
		switch (key) {
		case GLFW_KEY_ENTER:
			onStrChange(*this, representation);
			break;
		case GLFW_KEY_LEFT:
			cursorPosition = cursorPosition == 0 ? 0 : cursorPosition - 1;
			break;
		case GLFW_KEY_RIGHT: {
			cursorPosition = cursorPosition == representation.length ? representation.length : cursorPosition + 1;
			break;
		}
		case GLFW_KEY_BACKSPACE:
			if (representation.length > 0 && cursorPosition != 0) {
				representation.erase(cursorPosition - 1);
				cursorPosition--;
			}
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
			
				if (!isKeyDown(GLFW_KEY_LEFT_SHIFT, focusToken)) {
					c = std::tolower(c);
				}

				representation.insert(c, cursorPosition);
				cursorPosition++;
			}
		}
	}
}

void TextInput::render(const Shader& shader, const TextRenderer& textRenderer) {
	bt.render(shader, textRenderer, representation, isFocused ? focusedBackgroundColor : backgroundColor, textColor, cursorPosition);
	
	if (isFocused) {
		GLfloat cursorOffset = textRenderer.getStringWidth(representation.substring(0, cursorPosition), bt.scale);
		Rectangle cursorRect = { bt.rect.x + bt.padding + cursorOffset, bt.rect.y, 2, textRenderer.getFontSize() };
		cursorRect.render(shader, Vector4f { 1, 0, 0, 1 });
	}
}

void TextInput::free() {
	representation.deallocate();
}