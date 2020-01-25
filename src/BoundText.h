#ifndef BOUND_TEXT_H

#define BOUND_TEXT_H
#include "Rectangle.h"
#include "Shader.h"
#include "TextRenderer.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include <string>

struct BoundText {
	GLfloat scale = 1.f;
	GLfloat padding = 0.f;
	Rectangle rect;
};

void renderBoundText(const BoundText& bt, const Shader& shader, const TextRenderer& textRenderer, 
	std::string text, const Vector4f& backgroundColor, const Vector4f& textColor, GLint position = 0);
inline GLfloat getBoundTextHeight(const BoundText& bt, const TextRenderer& textRenderer) {
	return (textRenderer.getFontSize() + 2 * bt.padding) * bt.scale;
}

#endif // !BOUND_TEXT_H
