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
	case TextInputType::STRING: 
		break;
	case TextInputType::INT:
		if (textInput.lastValue.iVal != textInput.value.iVal) {
			textInput.representation.clear();
			textInput.lastValue.iVal = textInput.value.iVal;
			textInput.representation.addInt(textInput.value.iVal);
		} else {
			return;
		}
		break;
	case TextInputType::FLOAT:
		if (textInput.lastValue.fVal != textInput.value.fVal) {
			textInput.representation.clear();
			textInput.lastValue.fVal = textInput.value.fVal;
			textInput.representation.addFloat(textInput.value.fVal);
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

inline void onStrChange(TextInput& textInput) {
	String outVal = textInput.representation.toString();
    void* dataPtr = nullptr;
	switch (textInput.inputType) {
	case TextInputType::STRING: {
		dataPtr = outVal.getValue();
		break;
	}
	case TextInputType::INT: {
		textInput.value.iVal = outVal.toInteger();
	    dataPtr = &textInput.value.iVal;
		break;
	}
	case TextInputType::FLOAT: {
		textInput.value.fVal = outVal.toFloat();
	    dataPtr = &textInput.value.fVal;
		break;
	}
	default:
		// @TODO: Log error
		break;
	}

	setInternalRepresentation(textInput, true);
	if (textInput.mOnChange != nullptr) {
		textInput.mOnChange(dataPtr);
	}
	outVal.free();
}

void TextInput::update(const TextRenderer& textRenderer) {
	setInternalRepresentation(*this, false);
	bt.rect = getRectangle(*this, textRenderer);

	if (!isHovered) {
		if (bt.rect.isMouseHovered()) {
			isHovered = true;
			setCursorToText();
		}
	} else if (!bt.rect.isMouseHovered()) {
		isHovered = false;
		resetCursor();
	}

	bool isClicked = bt.rect.isClicked();
	if (isClicked) { // Move the mouse cursor pointer ot the proper position
		Vector2f mousePosition = getCursorPosition();
		GLfloat startX =  bt.rect.x + bt.padding;

		size_t charIdx = 0;
		while (mousePosition.x > startX && charIdx < representation.length) {
			startX += textRenderer.getCharWidth(representation.getCharAtIdx(charIdx), bt.scale);
			charIdx++;
		}
		cursorPosition = charIdx;
	}

	if (!isFocused) {
		if (isClicked) {
			focusToken = grabFocus();
			isFocused = true;
		}
	} else if (isLeftClickDown(focusToken) && !isHovered) {
		returnFocus(focusToken);
		isFocused = false;
		onStrChange(*this);
	}

	int key = getCurrentKeyDown(focusToken);
	int scancode = getCurrentScancode(focusToken);

	if (key > -1 && isKeyJustDown(key, focusToken)) {
		switch (key) {
		case GLFW_KEY_ENTER:
			onStrChange(*this);
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
				representation.removeAt(cursorPosition - 1, 1);
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
		GLfloat cursorOffset = 0;
		for (int idx = 0; idx < cursorPosition; idx++) {
			cursorOffset += textRenderer.getCharWidth(representation.getCharAtIdx(idx), bt.scale);
		}

		Rectangle cursorRect = { bt.rect.x + bt.padding + cursorOffset, bt.rect.y, 2, static_cast<float>(textRenderer.getFontSize()) };
		cursorRect.render(Vector4f { 1, 0, 0, 1 });
	}
}

void TextInput::free() {
	representation.free();
}
