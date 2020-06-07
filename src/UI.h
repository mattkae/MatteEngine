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

struct UI {
	TextRenderer mTextRenderer;
	List<UIContext> panels;
	UIEventProcessor* eventProcessor;

	void init();
	void update(double dt);
	void render() const;
	void free();

	size_t getNextPanelIdx();
	size_t showPanel(UIContext* context, int index);
	size_t showGlobalSelector(Scene& scene);
	size_t showModelEditor(Model* model);
	size_t showTerrainEditor(Terrain* terrain);

	void hidePanel(size_t index);
};