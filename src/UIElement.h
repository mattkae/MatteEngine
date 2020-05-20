#pragma once
#include "Button.h"
#include "TextInput.h"
#include "Label.h"

enum UIElementType {
	NO_ELEMENT = 0,
	BUTTON = 1,
	TEXT_INPUT = 2,
	LABEL = 3
};


union UIElementUnion {
	TextInput textInput;
	Button button;
	Label label;

	UIElementUnion() { };
	UIElementUnion(const UIElementUnion&c) { /*...*/ }
};

struct UIElement {
	UIElementType elementType;
	UIElementUnion element;
	void free();
};