#pragma once
#include "BoundText.h"
#include "Vector4f.h"
#include "Vector3f.h"
#include "Shader.h"
#include "MyString.h"

struct TextRenderer;
struct Label {
	BoundText bt;
	String text;
	Vector4f backgroundColor;
	Vector4f textColor;

	void build(
		GLfloat padding = 2.f, 
		Vector4f bkColor = { 0.1f, 0.1f, 0.1f, 1.f },
		Vector4f txtColor = { 1.f, 1.f, 1.f, 1.f },
		GLfloat scale = 1.f
	);
	void render(const Shader& shader, const TextRenderer& textRenderer);
	void free();
};