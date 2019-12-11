#define _USE_MATH_DEFINES
#include "Box.h"
#include "DebugArrow.h"
#include <cmath>

struct RenderableBox {
	GLuint vao;
	GLuint vbo;
};

RenderableBox globalBox;

const GLfloat boxVertexData[] = {
    -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f
};

inline void initializeRenderableBox() {
	glGenVertexArrays(1, &globalBox.vao);
	glGenBuffers(1, &globalBox.vbo);

	glBindVertexArray(globalBox.vao);
	glBindBuffer(GL_ARRAY_BUFFER, globalBox.vbo);

	glBufferData(GL_ARRAY_BUFFER, 105 * sizeof(boxVertexData), boxVertexData, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void renderBoxOutline(const Box& box, const Matrix4x4f& model, const Shader& shader) {
	if (globalBox.vao == 0) {
		initializeRenderableBox();
	}

	Vector3f scale = {
		fabsf(box.upperRight.x - box.lowerLeft.x) / 2.f,
		fabsf(box.upperRight.y - box.lowerLeft.y) / 2.f,
		fabsf(box.upperRight.z - box.lowerLeft.z) / 2.f,
	};

	Matrix4x4f scaledModel = scaleMatrix(model, scale);

	glBindVertexArray(globalBox.vao);
	setShaderMat4(shader, "uModel", scaledModel);
	setShaderVec3(shader, "uMaterial.emissive", glm::vec3(1, 0, 0));
	glDrawArrays(GL_LINE_LOOP, 0, 36);
	glBindVertexArray(0);
}

void updateDebugDebug(Box& box, Matrix4x4f& model) {

}

void renderDebugBox(const Box& box, const Matrix4x4f& model, const Shader& shader) {
	renderBoxOutline(box, model, shader);

	Vector3f rotation = { 0, 0, M_PI / 2 };
	Vector3f color = { 1, 0, 0 };
	renderDebugArrow(rotation, color, model, shader); 

	rotation = { 0, 0, 0 };
	color = { 0, 1, 0};
	renderDebugArrow(rotation, color, model, shader); 

	rotation = { M_PI / 2, 0, 0 };
	color = { 0, 0, 1 };
	renderDebugArrow(rotation, color, model, shader);
}