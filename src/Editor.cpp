#include "Editor.h"
#include "Input.h"
#include "Scene.h"

void Editor::initialize(Scene* inScene) {
	mScene = inScene;
	editorUI.initialize(mScene);
}

void Editor::free() {	
	editorUI.free();
}

void Editor::update(double dtMs) {
	if (!isEnabled) {
		return;
	}

	editorUI.ui.update(dtMs);
}

void Editor::render() const {
	if (!isEnabled) {
		return;
	}

	editorUI.ui.render();
}
