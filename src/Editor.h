#pragma once

#include "List.h"
#include "Light.h"
#include "EditorUI.h"

struct Scene;

struct Editor {
	void initialize(Scene* inScene);
	void free();
	void update(double dtMs);
	void render() const;
	inline void toggle() { isEnabled = !isEnabled ; };

	void toSceneFile(const char* outFile);
	void fromSceneFile(const char* inFile);

	EditorUI editorUI;
	Scene* mScene;
	List<Light> mLights;

	bool isEnabled = false;
};
