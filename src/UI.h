#pragma once
#include "Shader.h"
#include "Button.h"
#include "TextRenderer.h"
#include "TextInput.h"
#include "Panel.h"
#include "UIContext.h"
#include "List.h"
#include "Quaternion.h"

struct Model;
struct Terrain;
struct SceneUIController;

struct UI {
	SceneUIController* controller;
	Shader mOrthographicShader = 0;
	TextRenderer mTextRenderer;
	List<UIContext> panels;

	void init();
	void update(double dt);
	void render() const;
	void free();

	size_t getNextPanelIdx();
	size_t showPanel(UIContext* context, int index);
	size_t showModelSelector(Model* models, size_t numModels);
	size_t showModelEditor(Model* model);

	void hidePanel(size_t index);
};