#define _USE_MATH_DEFINES
#include "Box.h"
#include "DebugRender.h"
#include "Camera.h"
#include "List.h"
#include "Vertex.h"
#include <cmath>

inline void initializeRenderableBox(Box3D& box) {
	Vertex cubeVertices[8];
	cubeVertices[0].position = { box.upperRight.x, box.upperRight.y, box.upperRight.z };
	cubeVertices[0].normal = {  };
	cubeVertices[1].position = { box.lowerLeft.x, box.upperRight.y, box.upperRight.z };
	cubeVertices[1].normal = {  };
	cubeVertices[2].position = { box.lowerLeft.x, box.upperRight.y, box.lowerLeft.z };
	cubeVertices[2].normal = {  };
	cubeVertices[3].position = { box.upperRight.x, box.upperRight.y, box.lowerLeft.z };
	cubeVertices[3].normal = {  };

	cubeVertices[4].position = { box.upperRight.x, box.lowerLeft.y, box.upperRight.z };
	cubeVertices[4].normal = {  };
	cubeVertices[5].position = { box.lowerLeft.x, box.lowerLeft.y, box.upperRight.z };
	cubeVertices[5].normal = {  };
	cubeVertices[6].position = { box.lowerLeft.x, box.lowerLeft.y, box.lowerLeft.z };
	cubeVertices[6].normal = {  };
	cubeVertices[7].position = { box.upperRight.x, box.lowerLeft.y, box.lowerLeft.z };
	cubeVertices[7].normal = {  };

    GLint cubeIndices[] = {
	    0, 1, 3, //top 1
		3, 1, 2, //top 2
		2, 6, 7, //front 1
		7, 3, 2, //front 2
		7, 6, 5, //bottom 1
		5, 4, 7, //bottom 2
		5, 1, 4, //back 1
		4, 1, 0, //back 2
		4, 3, 7, //right 1
		3, 4, 0, //right 2
		5, 6, 2, //left 1
		5, 1, 2  //left 2
	};

	List<Vertex> vertices;
	vertices.growDynamically = false;
	vertices.setFromArray(cubeVertices, 8);

	List<GLint> indices;
	indices.growDynamically = false;
	indices.setFromArray(cubeIndices, 36);
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
