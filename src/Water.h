#pragma once
#include "Vector4f.h"
#include "Vector2f.h"
#include "Matrix4x4f.h"
#include "Shader.h"
#include "List.h"
#include "MathHelper.h"
#include "Material.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "MyString.h"

struct Scene;
struct Light;

struct WaterVertex {
	Vector3f	position;
	GLfloat		periodOffset;
	Vector2f    texCoords;
};

struct WaterParameters {
	GLint		width = 0;	// Width and height of the water area
	GLint		height = 0;
	GLfloat		verticesPerUnit = 0; // [0.0, 1.0] vertices per unit of width and height
	String      dudvTexturePath;
	String		normalMapPath;
};

struct Water {
	Scene*				scene = nullptr;
	Camera				camera;
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
	TextureFrameBuffer	reflectionFrameBuffer;
	TextureFrameBuffer  refractionFrameBuffer;
	GLuint              dudvTexture;
	GLuint				normalMap;
	GLfloat             dudvMoveFactor = 0;
	GLfloat             dudvSpeed = 0.03f;
	bool                isDisabled = false;

	void initialize(Scene* inScene, WaterParameters* waterIn);
	void update(float dt);
	void renderReflection();
	void render(Light* lights, unsigned int numLightsUsed) const;
	void free();
};
