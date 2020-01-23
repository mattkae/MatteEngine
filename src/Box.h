#pragma once
#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix4x4f.h"
#include "Camera.h"
#include "Ray.h"
#include "Material.h"
#include <algorithm>
#include "Shader.h"
#include <GL/glew.h>

struct Box {
	Vector4f lowerLeft;
	Vector4f upperRight;
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	Material material;
};

inline bool isInBox(const Box& box, const Vector4f& v, const Matrix4x4f& model) {
	Vector4f lowerLeft = mult(model, box.lowerLeft);
	Vector4f upperRight = mult(model, box.upperRight);

	return v.x >= lowerLeft.x 
		&& v.y >= lowerLeft.y
		&& v.z >= lowerLeft.z
		&& v.x <= upperRight.x 
		&& v.y <= upperRight.y
		&& v.z <= upperRight.z;
}

// Most of my understanding of this algorithm comes form here:
// https://tavianator.com/fast-branchless-raybounding-box-intersections-part-2-nans/
inline bool isBoxInRayPath(const Box& box, const Matrix4x4f& model, const Ray& ray) {
	const Vector4f& lowerLeft = mult(model, box.lowerLeft);
	const Vector4f& upperRight = mult(model, box.upperRight);
	const Vector3f& modelRayDirection = ray.direction;
	const Vector3f& rayPosition = ray.position;

	float xMin = (lowerLeft.x - rayPosition.x) / (modelRayDirection.x);
	float xMax = (upperRight.x - rayPosition.x) / (modelRayDirection.x);
	float min = std::min(xMin, xMax);
	float max = std::max(xMin, xMax);

	float yMin = (lowerLeft.y - rayPosition.y) / (modelRayDirection.y);
	float yMax = (upperRight.y - rayPosition.y) / (modelRayDirection.y);
	min = std::max(min, std::min(yMin, yMax));
	max = std::min(max, std::max(yMin, yMax));

	float zMin = (lowerLeft.z - rayPosition.z) / (modelRayDirection.z);
	float zMax = (upperRight.z - rayPosition.z) / (modelRayDirection.z);
	min = std::max(min, std::min(zMin, zMax));
	max = std::min(max, std::max(zMin, zMax));

	return max >= std::max(min, 0.f);
}

inline Vector4f getCenter(const Box& box) {
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

void updateBox(Box& box, const Matrix4x4f& model);
void renderBoxOutline(const Box& box, const Matrix4x4f& model, const Shader& shader);
GLfloat getDistanceFromCamera(const Box& box, const BetterCamera& camera, const Matrix4x4f& model);