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
	panel.maxWidth = GlobalAppState.floatWidth;
	panel.maxHeight = GlobalAppState.floatHeight;
	GLfloat panelWidth = panel.maxWidth * panel.percentageWidth;
	GLfloat panelHeight = panel.maxHeight * panel.percentageHeight;

	GLfloat x = getPositioning(panel.horizontal, panel.absolutePositioning.x, panel.maxWidth, panelWidth);
	GLfloat y = getPositioning(panel.vertical, panel.absolutePositioning.y, panel.maxHeight, panelHeight);

	panel.boundingRect = { x + panel.padding, y, panelWidth, panelHeight };
}

void renderPanel(const Panel& panel, const Shader& shader) {
	renderRectangle(panel.boundingRect, shader, panel.backgroundColor, panel.borderColor, panel.borderWidth);
}