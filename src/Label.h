#pragma once
#include "BoundText.h"
#include "Vector4f.h"
#include "Vector3f.h"
#include "Shader.h"
#include "String.h"

struct TextRenderer;
struct Label {
	BoundText bt;
	String text;
	Vector4f backgroundColor;
	Vector4f textColor;

	void render(const Shader& shader, const TextRenderer& textRenderer);
	void free();
};