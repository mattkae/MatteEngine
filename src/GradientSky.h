#pragma once
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "List.h"
#include <GL/glew.h>

struct Camera;

struct DomeVertex {
	Vector3f position;
};

struct GradientSky {
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLint numIndices = 0;

	Vector4f startColor;
	Vector4f endColor;
	Vector2f mixStartEnd;
	
	void initialize(float radius, float deltaPhi, float deltaTheta);
	void render(const Camera* camera);
	void free();
	
};
