#pragma once
#include "Matrix4x4f.h"
#include "Shader.h"
#include "Vector3f.h"
#include "Model.h"
#include "Box.h"
#include "Camera.h"
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

struct DebugArrow {
	Vector3f rotation;
	Vector3f color;
	Model model;
	Box boundingBox;

	bool isClicked = false;
};

void updateDebugArrow(DebugArrow& arrow, const Matrix4x4f& model);
void renderDebugArrow(const DebugArrow& arrow, const Shader& shader);

enum DebugClickState {
	NONE,
	X_AXIS,
	Y_AXIS,
	Z_AXIS
};

struct DebugModel {
	Box debugBox;
	Matrix4x4f model;
	DebugArrow xArrow = {
		{ 0, 0, M_PI / 2 },
		{ 1, 0, 0 }
	};
	DebugArrow yArrow = {
		{ 0, 0, 0 },
		{ 0, 1, 0 }
	};
	DebugArrow zArrow = {
		{ M_PI / 2, 0, 0 },
		{ 0, 0, 1 }
	};
	DebugClickState clickState = DebugClickState::NONE;
	int focusToken = -1;
};

void updateDebugModel(DebugModel& dbgModel, Matrix4x4f& model, const BetterCamera& camera);
void renderDebugModel(const DebugModel& dbgModel, const Matrix4x4f& model, const Shader& shader);