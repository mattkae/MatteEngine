#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../Shader.h"
#include "../Primitives/Rectangle.h"
#include "../GlobalApplicationState.h"

enum UIPositioning {
	ABSOLUTE = 0,
	LEFT = 1,
	CENTER = 2,
	RIGHT = 3,
};

struct Panel {
	GLfloat maxWidth = GlobalAppState.floatWidth;
	GLfloat maxHeight = GlobalAppState.floatHeight;

	Rectangle boundingRect = { 0, 0, GlobalAppState.floatWidth, GlobalAppState.floatHeight };
	GLfloat percentageWidth = 0.25f;
	GLfloat percentageHeight = 0.90f;
	UIPositioning horizontal = UIPositioning::ABSOLUTE;
	UIPositioning vertical = UIPositioning::ABSOLUTE;
	glm::vec2 absolutePositioning = glm::vec2(0);
	glm::vec4 backgroundColor = glm::vec4(1.0);
	glm::vec4 borderColor = glm::vec4(1.0);
	GLfloat borderWidth = 1.f;
	GLfloat padding = 2.f;
	bool isClosable = true;
};

void setPanelPosition(Panel& panel);
void renderPanel(const Panel& panel, const Shader& shader);