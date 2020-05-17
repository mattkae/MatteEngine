#pragma once
#include "Button.h"
#include "TextInput.h"
#include "Label.h"

enum UIElementType {
	BUTTON = 0,
	TEXT_INPUT = 1,
	LABEL = 2
};


struct UIElementUnion {
	TextInput textInput;
	Button button;
	Label label;
};

struct UIElement {
	UIElementUnion element;
	UIElementType elementType;
	void free();
};