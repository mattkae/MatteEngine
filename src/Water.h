#pragma once
#include "Vector4f.h"
#include "Vector2f.h"
#include "Matrix4x4f.h"
#include "Shader.h"
#include "List.h"
#include "MathHelper.h"
#include "Material.h"

struct Scene;
struct Camera;
struct Light;

struct WaterVertex {
	Vector3f	position;
	GLfloat		periodOffset;
};

struct WaterParameters {
	GLint		width = 0;	// Width and height of the water area
	GLint		height = 0;
	GLfloat		verticesPerUnit = 0; // [0.0, 1.0] vertices per unit of width and height
};

struct Water {
	Scene*				scene = nullptr;
	Camera*				camera = nullptr;
	Matrix4x4f			modelMatrix;
	List<WaterVertex>	vertices;
	GLuint				vao = 0;
	GLuint				vbo = 0;
	GLuint				ebo = 0;
	GLint				numIndices = 0;
	GLfloat				timePassedMs = 0;
	GLfloat				amplitude = 1.f;
	GLfloat				periodOffsetGradient = PI / 8.f;
	GLfloat				period = PI;
	Material			material;

	void initialize(Scene* inScene, Camera* inCamera, WaterParameters* waterIn);
	void update(float dt);
	void render(Light* lights, unsigned int numLightsUsed) const;
	void free();
};