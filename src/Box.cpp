#define _USE_MATH_DEFINES
#include "Box.h"
#include "DebugRender.h"
#include "Camera.h"
#include "List.h"
#include "Vertex.h"
#include <cmath>



inline void initializeRenderableBox(Box3D& box) {
	// Top
	Vector3f topVertices[4] = {
		{ box.lowerLeft.x, box.upperRight.y, box.lowerLeft.z },
		{ box.upperRight.x, box.upperRight.y, box.lowerLeft.z },
		{ box.upperRight.x, box.upperRight.y, box.upperRight.z },
		{ box.lowerLeft.x, box.upperRight.y, box.upperRight.z },
	};

	// Back
	Vector3f backVertices[4] = {
		{ box.lowerLeft.x, box.upperRight.y, box.upperRight.z },
		{ box.upperRight.x, box.upperRight.y, box.upperRight.z },
		{ box.upperRight.x, box.lowerLeft.y, box.upperRight.z },
		{ box.lowerLeft.x, box.lowerLeft.y, box.upperRight.z },
	};

	// Bottom
	Vector3f bottomVertices[4] = {
		{ box.lowerLeft.x, box.lowerLeft.y, box.upperRight.z },
		{ box.upperRight.x, box.lowerLeft.y, box.upperRight.z },
		{ box.upperRight.x, box.lowerLeft.y, box.lowerLeft.z },
		{ box.lowerLeft.x, box.lowerLeft.y, box.lowerLeft.z }
	};

	// Left
	Vector3f leftVertices[4] = {
		{ box.lowerLeft.x, box.lowerLeft.y, box.lowerLeft.z },
		{ box.lowerLeft.x, box.upperRight.y, box.lowerLeft.z },
		{ box.lowerLeft.x, box.upperRight.y, box.upperRight.z },
		{ box.lowerLeft.x, box.lowerLeft.y, box.upperRight.z }
	};

	// Front
	Vector3f frontVertices[4] = {
		{ box.lowerLeft.x, box.lowerLeft.y, box.lowerLeft.z },
		{ box.upperRight.x, box.lowerLeft.y, box.lowerLeft.z },
		{ box.upperRight.x, box.upperRight.y, box.lowerLeft.z },
		{ box.lowerLeft.x, box.upperRight.y, box.lowerLeft.z }
	};

	// Right
	Vector3f rightVertices[4] = {
		{ box.upperRight.x, box.lowerLeft.y, box.lowerLeft.z },
		{ box.upperRight.x, box.upperRight.y, box.lowerLeft.z },
		{ box.upperRight.x, box.upperRight.y, box.upperRight.z },
		{ box.upperRight.x, box.lowerLeft.y, box.upperRight.z }
	};
	
	List<Vertex> vertices;
	vertices.growDynamically = false;
	vertices.allocate(24);

	List<GLint> indices;
	indices.growDynamically = false;
	indices.allocate(1.5 * 24);

	for (int idx = 0; idx < 6; idx++) {
		Vector3f* faceVertices = NULL;
		Vector3f normal;
		if (idx == 0) {
			faceVertices = topVertices;
			normal = { 0, 1, 0 };
		} else if (idx == 1) {
			faceVertices = backVertices;
			normal = { 0, 0, -1 };
		} else if (idx == 2) {
			faceVertices = bottomVertices;
			normal = { 0, -1, 0 };
		}  else if (idx == 3) {
			faceVertices = leftVertices;
			normal = { 0, -1, 0 };
		} else if (idx == 4) {
			faceVertices = frontVertices;
			normal = { 0, 0, 1 };
		} else if (idx == 5) {
			faceVertices = rightVertices;
			normal = { 0, 1, 0 };
		} else {
			logger_error("Unable to find face vertices");
			continue;
		}

		for (int vertexIdx = 0; vertexIdx < 4; vertexIdx++) {
			Vertex newVertex;
			newVertex.position = faceVertices[vertexIdx];
			newVertex.normal = normal;
			vertices.add(newVertex);
		}

		int vertexStart = idx * 4;
		indices.add(vertexStart);
		indices.add(vertexStart + 1);
		indices.add(vertexStart + 2);
		indices.add(vertexStart + 2);
		indices.add(vertexStart + 3);
		indices.add(vertexStart);
	}

	box.mMesh.initializeFromVertices(&vertices, &indices);

	box.mMesh.material.diffuse = Vector3f{1.f, 1.f, 1.f};
	box.mMesh.material.ambient = Vector3f{1.f, 0.f, 0.f};

    vertices.deallocate();
	indices.deallocate();
}

void updateBox(Box3D& box) {
	if (box.mMesh.vao == 0) {
		initializeRenderableBox(box);
	}
}

void renderBoxOutline(const Box3D& box, const Matrix4x4f& model, const ModelUniformMapping& mapping, bool withMaterial) {
	setShaderMat4(mapping.MODEL, model);
    setShaderBool(mapping.DISABLE_BONES, true);
	box.mMesh.render(mapping.materialUniformMapping, withMaterial);
}

GLfloat getDistanceFromCamera(const Box3D& box, const Camera& camera, const Matrix4x4f& model) {
	GLfloat distanceFromLowerLeft = length(subtractVector(camera.position, mult(model, box.lowerLeft)));
	GLfloat distanceFromUpperRight = length(subtractVector(camera.position, mult(model, box.upperRight)));
	return std::min(distanceFromLowerLeft, distanceFromUpperRight);
}
