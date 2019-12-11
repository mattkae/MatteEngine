#define _USE_MATH_DEFINES
#include "DebugArrow.h"
#include "ObjModel.h"
#include <cmath>

DebugArrow globalArrow;

inline void initGlobalArrow() {
	loadFromObj((char*)"assets/simple_arrow.obj", globalArrow.model);
	initializeModel(globalArrow.model, globalArrow.boundingBox);
}

void renderDebugArrow(Vector3f rotation, Vector3f color, const Matrix4x4f& model, const Shader& shader) {
	if (globalArrow.model.meshes.size() == 0) {
		initGlobalArrow();
	}

	for (auto& mesh : globalArrow.model.meshes) {
		mesh.material.emissive = color;
		mesh.material.ambient = color;
	}

	globalArrow.model.model = getIdentity();
	globalArrow.model.model = scaleMatrix(globalArrow.model.model, 0.1f);

	// Rotate and scale
	Vector4f upperRight = mult(globalArrow.model.model, globalArrow.boundingBox.upperRight);
	Vector4f lowerLeft = mult(globalArrow.model.model, globalArrow.boundingBox.lowerLeft);
	Vector3f translation = scale(Vector3f{0, 1, 0}, abs(upperRight.y - lowerLeft.y) / 2.f);

	globalArrow.model.model = translateMatrix(globalArrow.model.model, translation);
	globalArrow.model.model = rotate(globalArrow.model.model, rotation.x, rotation.y, rotation.z);
	globalArrow.model.model = mult(globalArrow.model.model, model);
	renderModel(globalArrow.model, shader);
}

void freeDebugArrow() {

}