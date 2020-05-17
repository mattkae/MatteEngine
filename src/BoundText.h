#ifndef BOUND_TEXT_H

#define BOUND_TEXT_H
#include "Rectangle.h"
#include "Shader.h"
#include "TextRenderer.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "String.h"

struct BoundText {
	GLfloat scale = 1.f;
	GLfloat padding = 0.f;
	Rectangle rect;

	void render(const Shader& shader, const TextRenderer& textRenderer, 
		String text, const Vector4f& backgroundColor, const Vector4f& textColor, GLint position = 0);
	
	inline GLfloat getBoundTextHeight(const TextRenderer& textRenderer) const {
		return (textRenderer.getFontSize() + 2 * padding) * scale;
	}
};

#endif // !BOUND_TEXT_H
