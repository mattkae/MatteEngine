#pragma once
#include "Panel.h"
#include "../Input.h"
#include "../GlobalApplicationState.h"
#include "../Primitives/Rectangle.h"

GLfloat getPositioning(UIPositioning positioning, GLfloat absolutePosition, GLfloat windowDimension, GLfloat panelDimension) {
	switch (positioning) {
	case UIPositioning::ABSOLUTE:
		return absolutePosition;
	case UIPositioning::CENTER:
		return (windowDimension - panelDimension) / 2.f;
	case UIPositioning::LEFT: // TOP
		return 0;
	case UIPositioning::RIGHT: // DOWN
		return windowDimension - panelDimension;
	default:
		return 0;
	}
}

void setPanelPosition(Panel& panel) {
	GLfloat panelWidth = GlobalAppState.floatWidth * panel.percentageWidth;
	GLfloat panelHeight = GlobalAppState.floatHeight * panel.percentageHeight;

	GLfloat x = getPositioning(panel.horizontal, panel.absolutePositioning.x, GlobalAppState.floatWidth, panelWidth);
	GLfloat y = getPositioning(panel.vertical, panel.absolutePositioning.y, GlobalAppState.floatHeight, panelHeight);

	panel.boundingRect = { x, y, panelWidth, panelHeight };
}

void renderPanel(const Panel& panel, const Shader& shader) {
	renderRectangle(panel.boundingRect, shader, panel.backgroundColor, panel.borderColor, panel.borderWidth);
}