#pragma once
#include "Vector3f.h"
#include "Vector4f.h"
#include "List.h"
#include <GL/glew.h>

struct Camera;

struct DomeVertex {
	Vector3f position;
	Vector4f color = { 1.0f, 0.0f, 0.0f, 1.f };
};

struct GradientSky {
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLint numIndices = 0;
	
	void initialize(float radius, float deltaPhi, float deltaTheta, Vector4f startColor, Vector4f endColor, float mixPercent);
	void render(const Camera* camera);
	void free();
	
};
