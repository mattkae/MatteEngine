#pragma once
#include "Panel.h"
#include "Constants.h"

GLfloat getPositioning(UIPositioning positioning, GLfloat absolutePosition, GLint windowDimension, GLfloat panelDimension) {
	switch (positioning) {
	case UIPositioning::ABSOLUTE:
		return absolutePosition;
	case UIPositioning::CENTER:
		return static_cast<GLfloat>(windowDimension) / 2.f - panelDimension / 2.f;
	case UIPositioning::LEFT: // TOP
		return 0;
	case UIPositioning::RIGHT: // DOWN
		return static_cast<GLfloat>(windowDimension) - panelDimension;
	}
}

void initializePanel(Panel& panel) {
	GLfloat panelWidth = static_cast<GLfloat>(Constants.width) * panel.percentageWidth;
	GLfloat panelHeight = static_cast<GLfloat>(Constants.height) * panel.percentageHeight;

	GLfloat x = getPositioning(panel.horizontal, panel.absolutePositioning.x, Constants.width, panelWidth);
	GLfloat y = getPositioning(panel.vertical, panel.absolutePositioning.y, Constants.height, panelHeight);

	panel.rect.r = { x, y, panelWidth, panelHeight };
	initializeRenderableRectangle(panel.rect);
}

void renderPanel(const Panel& panel, const Shader& shader) {
	renderRenderableRectangle(panel.rect, shader, glm::vec3(panel.backgroundColor.x, panel.backgroundColor.y, panel.backgroundColor.z));
}