#pragma once
#include "Vector4f.h"
#include "Matrix4x4f.h"
#include "Input.h"
#include "Camera.h"
#include "GlobalApplicationState.h"

struct Ray {
	Vector3f direction;
	Vector3f position;
};

Ray clickToRay(const Camera& camera);