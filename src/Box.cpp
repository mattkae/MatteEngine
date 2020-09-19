#define _USE_MATH_DEFINES
#include "Box.h"
#include "DebugRender.h"
#include "Camera.h"
#include <cmath>

inline void initializeRenderableBox(Box3D& box) {
	static const GLfloat cubeVertices[] = {
		box.lowerLeft.x, box.lowerLeft.y, box.upperRight.z,
		box.upperRight.x, box.lowerLeft.y, box.upperRight.z,
		box.lowerLeft.x,  box.upperRight.y,  box.upperRight.z,
		box.upperRight.x,  box.upperRight.y,  box.upperRight.z,
		box.lowerLeft.x, box.lowerLeft.y, box.lowerLeft.z,
		box.upperRight.x, box.lowerLeft.y, box.lowerLeft.z,
		box.lowerLeft.x,  box.upperRight.y, box.lowerLeft.z,
		box.upperRight.x,  box.upperRight.y, box.lowerLeft.z
	};

	static const GLuint cubeIndices[] = {
		0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
	};

	glGenVertexArrays(1, &box.vao);
	glGenBuffers(1, &box.vbo);
	glGenBuffers(1, &box.ebo);

	glBindVertexArray(box.vao);

	glBindBuffer(GL_ARRAY_BUFFER, box.vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cubeVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 14 * sizeof(GLuint), cubeIndices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	box.material.transparency = 0.5f;
	box.material.ambient = Vector3f{1.f, 0.f, 0.f};
}

void updateBox(Box3D& box, const Matrix4x4f& model) {
	if (box.vao == 0) {
		initializeRenderableBox(box);
	}
}

void renderBoxOutline(const Box3D& box, const Matrix4x4f& model, const ModelUniformMapping& mapping) {
	setShaderMat4(mapping.MODEL, model);
	box.material.render(mapping.materialUniformMapping);

	glBindVertexArray(box.vao);
    glDrawElements(GL_TRIANGLE_STRIP, 14, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

GLfloat getDistanceFromCamera(const Box3D& box, const Camera& camera, const Matrix4x4f& model) {
	GLfloat distanceFromLowerLeft = length(subtractVector(camera.position, mult(model, box.lowerLeft)));
	GLfloat distanceFromUpperRight = length(subtractVector(camera.position, mult(model, box.upperRight)));
	return std::min(distanceFromLowerLeft, distanceFromUpperRight);
}