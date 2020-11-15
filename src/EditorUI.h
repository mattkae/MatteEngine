#pragma once
#include "UIContext.h"
#include "UI.h"
#include "UIEventProcessor.h"

struct Scene;
struct Model;

struct EditorUI {
	UIContext primaryUI;
	UIContext modelUI;
	UIContext terrainUI;
	UIContext textureDebuggerUI;
	UI ui;

	void initialize(Scene* scene);

	void initPrimaryUI(Scene* scene);
	void openModelUI(Model* model);
	void initTerrainUI(Scene* scene);
	void initTextureDebuggerUI();

	void free();
};