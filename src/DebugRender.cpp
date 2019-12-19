#include "DebugRender.h"
#include "ObjModel.h"
#include "Input.h"
#include "Ray.h"

inline void initArrow(DebugArrow& arrow) {
	loadFromObj((char*)"assets/simple_arrow.obj", arrow.model);
	initializeModel(arrow.model, arrow.boundingBox);
	for (auto& mesh : arrow.model.meshes) {
		mesh.material.emissive = arrow.color;
		mesh.material.ambient = arrow.color;
	}
}

void updateDebugArrow(DebugArrow& arrow, const Matrix4x4f& model) {
	if (arrow.model.meshes.size() == 0) {
		initArrow(arrow);
	}
	arrow.model.model = getIdentity();
	arrow.model.model = scaleMatrix(arrow.model.model, 0.1f);

	// Rotate and scale
	Vector4f upperRight = mult(arrow.model.model, arrow.boundingBox.upperRight);
	Vector4f lowerLeft = mult(arrow.model.model, arrow.boundingBox.lowerLeft);
	Vector3f translation = scale(Vector3f{0, 1, 0}, abs(upperRight.y - lowerLeft.y) / 2.f);

	arrow.model.model = translateMatrix(arrow.model.model, translation);
	arrow.model.model = rotate(arrow.model.model, arrow.rotation.x, arrow.rotation.y, arrow.rotation.z);
	arrow.model.model = mult(arrow.model.model, model);
}

void renderDebugArrow(const DebugArrow& arrow, const Shader& shader) {
	renderModel(arrow.model, shader);
}

inline void changeArrowColor(DebugArrow& arrow, Vector3f color) {
	arrow.color = color;
	for (auto& mesh : arrow.model.meshes) {
		mesh.material.emissive = arrow.color;
		mesh.material.ambient = arrow.color;
	}
}

void updateDebugModel(DebugModel& dbgModel, Matrix4x4f& model, const BetterCamera& camera) {
	dbgModel.model = copyMatrix(model);
	updateDebugArrow(dbgModel.xArrow, dbgModel.model);
	updateDebugArrow(dbgModel.yArrow, dbgModel.model);
	updateDebugArrow(dbgModel.zArrow, dbgModel.model);

	if (dbgModel.clickState == DebugClickState::NONE) {
		if (isLeftClickDown()) {
			Vector4f rayWorld = clickToRay(camera);
			if (isBoxInRayPath(dbgModel.debugBox, model, rayWorld, camera)) {
				GLfloat distanceFromCamera = -1;

				if (isBoxInRayPath(dbgModel.xArrow.boundingBox, dbgModel.xArrow.model.model, rayWorld, camera)) {
					dbgModel.clickState = DebugClickState::X_AXIS;
					distanceFromCamera = getDistanceFromCamera(dbgModel.xArrow.boundingBox, camera, dbgModel.xArrow.model.model);
				}
				
				if (isBoxInRayPath(dbgModel.yArrow.boundingBox, dbgModel.yArrow.model.model, rayWorld, camera)) {
					GLfloat nextDistanceFromCamera = getDistanceFromCamera(dbgModel.yArrow.boundingBox, camera, dbgModel.yArrow.model.model);
					if (distanceFromCamera < 0 || nextDistanceFromCamera < distanceFromCamera) {
						dbgModel.clickState = DebugClickState::Y_AXIS;
						distanceFromCamera = nextDistanceFromCamera;
					}
				}
				
				if (isBoxInRayPath(dbgModel.zArrow.boundingBox, dbgModel.zArrow.model.model, rayWorld, camera)) {
					GLfloat nextDistanceFromCamera = getDistanceFromCamera(dbgModel.zArrow.boundingBox, camera, dbgModel.zArrow.model.model);
					if (distanceFromCamera < 0 || nextDistanceFromCamera < distanceFromCamera) {
						dbgModel.clickState = DebugClickState::Z_AXIS;
					}
				}

				switch (dbgModel.clickState) {
				case DebugClickState::X_AXIS:
					changeArrowColor(dbgModel.xArrow, Vector3f{1, 1, 0});
					break;
				case DebugClickState::Y_AXIS:
					changeArrowColor(dbgModel.yArrow, Vector3f{1, 1, 0});
					break;
				case DebugClickState::Z_AXIS:
					changeArrowColor(dbgModel.zArrow, Vector3f{1, 1, 0});
					break;
				}

				dbgModel.lastMousePos = getCursorPosition();
			}
		}
	} else if (!isLeftClickDown()) {
		switch (dbgModel.clickState) {
		case DebugClickState::X_AXIS:
			changeArrowColor(dbgModel.xArrow, Vector3f{1, 0, 0});
			break;
		case DebugClickState::Y_AXIS:
			changeArrowColor(dbgModel.yArrow, Vector3f{0, 1, 0});
			break;
		case DebugClickState::Z_AXIS:
			changeArrowColor(dbgModel.zArrow, Vector3f{0, 0, 1});
			break;
		}

		dbgModel.clickState = DebugClickState::NONE;
	} else {
		Point nextCursorPos = getCursorPosition();
		Point diff = { nextCursorPos.x - dbgModel.lastMousePos.x, nextCursorPos.y - dbgModel.lastMousePos.y };
		dbgModel.lastMousePos = nextCursorPos;

		switch (dbgModel.clickState) {
		case DebugClickState::X_AXIS:
			model = translateMatrix(model, Vector3f{diff.x, 0, 0});
			break;
		case DebugClickState::Y_AXIS:
			model = translateMatrix(model, Vector3f{0, diff.y, 0});
			break;
		case DebugClickState::Z_AXIS:
			model = translateMatrix(model, Vector3f{0, 0, diff.x});
			break;
		}
	}
}

void renderDebugModel(const DebugModel& dbgModel, const Matrix4x4f& model, const Shader& shader) {
	renderBoxOutline(dbgModel.debugBox, model, shader);
	renderDebugArrow(dbgModel.xArrow, shader);
	renderDebugArrow(dbgModel.yArrow, shader);
	renderDebugArrow(dbgModel.zArrow, shader);
}