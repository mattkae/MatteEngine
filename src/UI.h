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
struct UIEventProcessor;

struct TextureDebugScreen {
	bool show = false;
	Rectangle rectangle = { 8.f, 0.f, 240.f, 150.f };
	GLuint texture;

	void render() const;
};

struct UI {
	TextRenderer mTextRenderer;
	List<UIContext*> panels;
	UIEventProcessor eventProcessor;
	TextureDebugScreen textureDebugger;

	void init();
	void update(double dt);
	void render() const;
	void free();

	void addPanel(UIContext* panel);
	// @TODO: Add remove on the panel
};

namespace UIBuilder {
	void addStandardLabel(const char* str, UIContext& context);
	void addFieldLabel(const char* str, UIContext& context);
	void addTextInput(UIContext& context,
		TextInputValue value, 
		TextInputType inputType,
		UIEventType eventType);
};