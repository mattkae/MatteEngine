#include "UI.h"
#include "Scene.h"
#include "GlobalApplicationState.h"
#include <glm/gtc/matrix_transform.hpp>

void initUI(UI& ui) {
	ui.mOrthographicShader = loadShader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	ui.mTextRenderer.initialize(16, (GLchar*)"assets/fonts/consola.ttf");
}

void updateUI(UI& ui, double dt) {
	for (size_t idx = 0; idx < ui.numPanels; idx++) {
		UIContext& panel = ui.panels[idx];
		if (panel.shouldOpen) {
			for (size_t otherIdx = 0; otherIdx < ui.numPanels; otherIdx++) {
				// This only runs once every time you openm a new context, so we can afford to be slow but robust
				if (ui.panels[otherIdx].isActive
					&& ui.panels[otherIdx].panel.vertical == panel.panel.vertical
					&& ui.panels[otherIdx].panel.horizontal == panel.panel.horizontal) {
					ui.panels[otherIdx].isActive = false;
				}
			}
		}

		updateUIContext(panel, ui.mTextRenderer);
	}
}


void renderUI(const UI& ui) {
	useShader(ui.mOrthographicShader);
	Matrix4x4f projection = getOrthographicProjection(0.0f, GlobalAppState.floatWidth, 0.0f, GlobalAppState.floatHeight);
	setShaderMat4(ui.mOrthographicShader, "uProjection", projection);
	for (size_t idx = 0; idx < ui.numPanels; idx++) {
		renderUIContext(ui.panels[idx], ui.mOrthographicShader, ui.mTextRenderer);
	}
}

void freeUI(UI& ui) {
	for (size_t idx = 0; idx < ui.numPanels; idx++) {
		freeUIContext(ui.panels[idx]);
	}

	delete[] ui.panels;
	ui.numPanels = 0;
}

void openModelPanel(UI& ui, size_t index) {
	size_t modelIndex = ui.modelOffset + index;
	ui.panels[modelIndex].shouldOpen = true;
}