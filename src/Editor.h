#pragma once

#include "List.h"
#include "Light.h"
#include "DebugRender.h"
#include "UI/UI.h"

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

	Scene* mScene;
	DebugModel mDebugModel;
	int mSelectedModelIdx = -1;
	UI::PrimaryContext context;

	bool isEnabled = false;
};
