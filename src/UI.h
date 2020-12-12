#pragma once
#include "Button.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "UIContext.h"
#include "List.h"
#include "Quaternion.h"

struct Model;
struct Terrain;

struct UI {
	TextRenderer mTextRenderer;
	List<UIContext*> panels;

	void init();
	void update(double dt);
	void render() const;
	void free();

	void addPanel(UIContext* panel);
	// @TODO: Add remove on the panel
};

namespace UIBuilder {
	void createStandardLabel(UIContext* context, const char* str);
	void createFieldLabel(UIContext* context, const char* str);
	void createTextInput(UIContext* context, TextInputValue value, TextInputType inputType, void (*onChange)(void*));
	void createStandardButton(UIContext* context, void (*onClick)(int), int data = -1);
};
