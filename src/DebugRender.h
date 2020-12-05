#pragma once
#include "Matrix4x4f.h"
#include "Shader.h"
#include "Vector3f.h"
#include "Model.h"
#include "Box.h"
#include "Camera.h"
#include "MathHelper.h"
#include "ShaderUniformMapping.h"

struct DebugArrow {
	Vector3f rotation;
	Vector3f color;
	Model model;
	Box3D boundingBox;

	bool isClicked = false;
	void update(Box3D& debugBox, const Matrix4x4f& model);
	void render(const ModelUniformMapping& mapping) const;
};


enum class DebugClickState {
	NONE,
	X_AXIS,
	Y_AXIS,
	Z_AXIS
};

struct DebugModel {
	Box3D debugBox;
	Matrix4x4f model;
	DebugArrow xArrow = {
		{ 0, 0, PI / 2 },
		{ 1, 0, 0 }
	};
	DebugArrow yArrow = {
		{ 0, 0, 0 },
		{ 0, 1, 0 }
	};
	DebugArrow zArrow = {
		{ PI / 2, 0, 0 },
		{ 0, 0, 1 }
	};
	DebugClickState clickState = DebugClickState::NONE;
	int focusToken = -1;

	void update(Matrix4x4f& model, const Camera& camera);
	void render(const Matrix4x4f& model, const ModelUniformMapping& mapping) const;
	void free();
};
