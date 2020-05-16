#include "UI.h"
#include "Scene.h"
#include "GlobalApplicationState.h"

void UI::init() {
	mOrthographicShader = loadShader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	mTextRenderer.initialize(16, (GLchar*)"assets/fonts/consola.ttf");
	panels.allocate(8);
}

void UI::update(double dt) {
	for (size_t idx = 0; idx < panels.numElements; idx++) {
		UIContext& panel = panels[idx];
		if (panel.shouldOpen) {
			for (size_t otherIdx = 0; otherIdx < panels.numElements; otherIdx++) {
				// This only runs once every time you open a new context, so we can afford to be slow but robust
				if (panels[otherIdx].isActive
					&& panels[otherIdx].panel.vertical == panel.panel.vertical
					&& panels[otherIdx].panel.horizontal == panel.panel.horizontal) {
					panels[otherIdx].isActive = false;
				}
			}
		}

		panel.update(mTextRenderer);
	}
}


void UI::render() const {
	useShader(mOrthographicShader);
	Matrix4x4f projection = getOrthographicProjection(0.0f, GlobalAppState.floatWidth, 0.0f, GlobalAppState.floatHeight);
	setShaderMat4(mOrthographicShader, "uProjection", projection);

	for (size_t idx = 0; idx < panels.numElements; idx++) {
		panels[idx].render(mOrthographicShader, mTextRenderer);
	}
}

void UI::free() {
	for (size_t idx = 0; idx < panels.numElements; idx++) {
		panels[idx].free();
	}

	panels.free();

	if (mOrthographicShader > 0) {
		glDeleteProgram(mOrthographicShader);
	}
}