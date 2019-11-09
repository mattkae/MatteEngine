#include "UI.h"
#include "../Scene.h"
#include "../GlobalApplicationState.h"
#include <glm/glm.hpp>

UI::UI() {
}

void UI::update(double dt) {
	for (size_t idx = 0; idx < context.size; idx++) {
		updateUIContext(context.elements[idx], mTextRenderer);
	}
}

void UI::generate() {
	mOrthographicShader = loadShader("src/shaders/Orthographic.vert","src/shaders/Orthographic.frag");
	mTextRenderer.initialize(16, (GLchar*)"assets/fonts/consola.ttf");
}

void UI::render() const {
	useShader(mOrthographicShader);
	glm::mat4 projection = glm::ortho(0.0f, GlobalAppState.floatWidth, 0.0f, GlobalAppState.floatHeight);
	setShaderMat4(mOrthographicShader, "uProjection", projection);
	for (size_t idx = 0; idx < context.size; idx++) {
		renderUIContext(context.elements[idx], mOrthographicShader, mTextRenderer);
	}
}