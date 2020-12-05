#pragma once

#include "List.h"
#include "Light.h"
#include "EditorUI.h"
#include "DebugRender.h"

struct Scene;

struct Editor {
	void initialize(Scene* inScene);
	void free();
	void update(double dtMs);
	void render() const;
	inline void toggle() { isEnabled = !isEnabled ; };
	int castRayToModel();

	void toSceneFile(const char* outFile);
	void fromSceneFile(const char* inFile);

	EditorUI editorUI;
	Scene* mScene;
	List<Light> mLights;
	DebugModel mDebugModel;
	int mSelectedModelIdx = -1;

	bool isEnabled = false;
};
