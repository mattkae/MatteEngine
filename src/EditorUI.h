#pragma once
#include "UIContext.h"
#include "UI.h"

struct Scene;
struct Model;
struct Light;

struct EditorUI {
	UIContext primaryUI;
	UIContext terrainUI;
	UIContext textureDebuggerUI;
	UI ui;
	Scene* mScene = nullptr;

	void initialize(Scene* scene);

	void initPrimaryUI(Scene* scene);
	void initTerrainUI(Scene* scene);
	void initTextureDebuggerUI();

	void free();
};
