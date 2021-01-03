#pragma once
#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix4x4f.h"
#include "Ray.h"
#include "MathHelper.h"
#include "ShaderUniformMapping.h"
#include "Mesh.h"
#include <GL/glew.h>

struct Camera;

struct Box3D {
	Vector4f lowerLeft;
	Vector4f upperRight;
    Mesh mMesh;
};

inline bool isInBox(const Box3D& box, const Vector4f& v, const Matrix4x4f& model) {
	Vector4f lowerLeft = model * box.lowerLeft;
	Vector4f upperRight = model * box.upperRight;

	return v.x >= lowerLeft.x 
		&& v.y >= lowerLeft.y
		&& v.z >= lowerLeft.z
		&& v.x <= upperRight.x 
		&& v.y <= upperRight.y
		&& v.z <= upperRight.z;
}

// Most of my understanding of this algorithm comes form here:
// https://tavianator.com/fast-branchless-raybounding-box-intersections-part-2-nans/
inline bool isBoxInRayPath(const Box3D& box, const Matrix4x4f& model, const Ray& ray) {
	const Vector4f& lowerLeft = model * box.lowerLeft;
	const Vector4f& upperRight = model * box.upperRight;
	const Vector3f& modelRayDirection = ray.direction;
	const Vector3f& rayPosition = ray.position;

	float min = (lowerLeft.x - rayPosition.x) / (modelRayDirection.x);
	float max = (upperRight.x - rayPosition.x) / (modelRayDirection.x);
	if (min > max) {
		SWAP(min, max, float);
	}

	float yMin = (lowerLeft.y - rayPosition.y) / (modelRayDirection.y);
	float yMax = (upperRight.y - rayPosition.y) / (modelRayDirection.y);
	if (yMin > yMax) {
		SWAP(yMin, yMax, float);
	}

	if (min > yMax || yMin > max) {
		return false;
	}
	
	min = MAX(min, yMin);
	max = MIN(max, yMax);

	float zMin = (lowerLeft.z - rayPosition.z) / (modelRayDirection.z);
	float zMax = (upperRight.z - rayPosition.z) / (modelRayDirection.z);
	if (zMin > zMax) {
		SWAP(zMin, zMax, float);
	}

	if (min > zMax || zMin > max) {
		return false;
	}
	
	min = MAX(min, zMin);
	max = MIN(max, zMax);
	return max >= MAX(min, 0.f);
}

inline Vector4f getCenter(const Box3D& box) {
	float xSize = (box.upperRight.x - box.lowerLeft.x) / 2.f;
	float ySize = (box.upperRight.y - box.lowerLeft.y) / 2.f;
	float zSize = (box.upperRight.z - box.lowerLeft.z) / 2.f;
	return { 
		box.lowerLeft.x + xSize, 
		box.lowerLeft.y + ySize, 
		box.lowerLeft.z + zSize, 
		1.f 
	};
}

void updateBox(Box3D& box);
void renderBoxOutline(const Box3D& box, const Matrix4x4f& model, const ModelUniformMapping& mapping, bool withMaterial);
GLfloat getDistanceFromCamera(const Box3D& box, const Camera& camera, const Matrix4x4f& model);
