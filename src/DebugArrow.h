#pragma once
#include "Matrix4x4f.h"
#include "Shader.h"
#include "Vector3f.h"
#include "Model.h"

struct DebugArrow {
	Vector3f rotation;
	Vector3f color;
	Model model;
	Box boundingBox;
};

void renderDebugArrow(Vector3f rotation, Vector3f color, const Matrix4x4f& model, const Shader& shader);
void freeDebugArrow();