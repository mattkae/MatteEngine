#include "UI.h"
#include "Scene.h"
#include "GlobalApplicationState.h"
#include <glm/glm.hpp>

void initUI(UI& ui) {
	ui.mOrthographicShader = loadShader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	ui.mTextRenderer.initialize(16, (GLchar*)"assets/fonts/consola.ttf");
}

void updateUI(UI& ui, double dt) {
	for (size_t idx = 0; idx < ui.context.size; idx++) {
		if (ui.context.elements[idx].shouldOpen) {
			for (size_t otherIdx = 0; otherIdx < ui.context.size; otherIdx++) {
				// This only runs once every time you openm a new context, so we can afford to be slow but robust
				if (ui.context.elements[otherIdx].isActive
					&& ui.context.elements[otherIdx].panel.vertical == ui.context.elements[idx].panel.vertical
					&& ui.context.elements[otherIdx].panel.horizontal == ui.context.elements[idx].panel.horizontal) {
					ui.context.elements[otherIdx].isActive = false;
				}
			}
		}

		updateUIContext(ui.context.elements[idx], ui.mTextRenderer);
	}
}


void renderUI(const UI& ui) {
	useShader(ui.mOrthographicShader);
	glm::mat4 projection = glm::ortho(0.0f, GlobalAppState.floatWidth, 0.0f, GlobalAppState.floatHeight);
	setShaderMat4(ui.mOrthographicShader, "uProjection", projection);
	for (size_t idx = 0; idx < ui.context.size; idx++) {
		renderUIContext(ui.context.elements[idx], ui.mOrthographicShader, ui.mTextRenderer);
	}
}

void freeUI(UI& ui) {
	for (size_t idx = 0; idx < ui.context.size; idx++) {
		freeUIContext(ui.context.elements[idx]);
	}

	deallocateArray(ui.context);
}