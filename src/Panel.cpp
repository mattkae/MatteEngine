#pragma once
#include "Panel.h"
#include "GlobalApplicationState.h"

GLfloat getPositioning(UIPositioning positioning, GLfloat absolutePosition, GLfloat windowDimension, GLfloat panelDimension) {
	switch (positioning) {
	case UIPositioning::ABSOLUTE:
		return absolutePosition;
	case UIPositioning::CENTER:
		return windowDimension / 2.f - panelDimension / 2.f;
	case UIPositioning::LEFT: // TOP
		return 0;
	case UIPositioning::RIGHT: // DOWN
		return windowDimension - panelDimension;
	}
}

void initializePanel(Panel& panel) {
	GLfloat panelWidth = panel.boundingRect.w * panel.percentageWidth;
	GLfloat panelHeight = panel.boundingRect.h * panel.percentageHeight;

	GLfloat x = getPositioning(panel.horizontal, panel.absolutePositioning.x, panel.boundingRect.w, panelWidth);
	GLfloat y = getPositioning(panel.vertical, panel.absolutePositioning.y, panel.boundingRect.h, panelHeight);

	panel.rect.r = { x, y, panelWidth, panelHeight };
	initializeRenderableRectangle(panel.rect);
}

void renderPanel(const Panel& panel, const Shader& shader) {
	renderRenderableRectangle(panel.rect, shader, glm::vec3(panel.backgroundColor.x, panel.backgroundColor.y, panel.backgroundColor.z));
}

// Window
	// Panel 100% 50% 
		// Button 50% 50% xOffset -> Panel.x + mX
		// TextInput 50% 50%
		// Panel 100% 50% xOffset -> Panel.xOffset = mX
			// Button 100% 100%