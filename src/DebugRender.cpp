#include "DebugRender.h"
#include "Input.h"
#include "Ray.h"
#include "Plane.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "Box.h"

inline void changeArrowColor(DebugArrow& arrow, Vector3f color) {
	arrow.color = color;
	for (int meshIdx = 0; meshIdx < arrow.model.numMeshes; meshIdx++) {
		arrow.model.meshes[meshIdx].material.emissive = arrow.color;
		arrow.model.meshes[meshIdx].material.ambient = arrow.color;
		arrow.model.meshes[meshIdx].material.diffuse = arrow.color;
	}
}

inline void initArrow(DebugArrow& arrow) {
	/*ModelLoader::ModelLoadResult retval = ModelLoader::loadSerializedModel((char*)"assets/models/Arrow/simple_arrow.mattl");
	arrow.model = retval.model;
	arrow.boundingBox = retval.box;
	changeArrowColor(arrow, arrow.color);*/
}

void updateDebugArrow(DebugArrow& arrow, Box& box, const Matrix4x4f& model) {
	arrow.model.model = getIdentity();
	if (arrow.model.numMeshes == 0) {
		initArrow(arrow);
	}

	// Rotate and scale per its direction
	Vector4f upperRight = arrow.boundingBox.upperRight;
	Vector4f lowerLeft = arrow.boundingBox.lowerLeft;
	Vector3f translation = scale(Vector3f{0, 1, 0}, abs(upperRight.y - lowerLeft.y) / 2.f);

	arrow.model.model = translateMatrix(arrow.model.model, translation);
	arrow.model.model = rotate(arrow.model.model, arrow.rotation.x, arrow.rotation.y, arrow.rotation.z);
	arrow.model.model = mult(arrow.model.model, model);

	// Center inside of the bounding box
	Vector4f boxUpperRight = mult(model, box.upperRight);
	Vector4f boxLowerLeft = mult(model, box.lowerLeft);
	Vector3f centerOfBox = Vector3f{ 0, fabsf(boxUpperRight.y - boxLowerLeft.y) / 2.f, 0 };
	arrow.model.model = translateMatrix(arrow.model.model, centerOfBox);
}

void renderDebugArrow(const DebugArrow& arrow, const ModelUniformMapping& mapping) {
	arrow.model.render(mapping);
}

void updateDebugModel(DebugModel& dbgModel, Matrix4x4f& model, const Camera& camera) {
	dbgModel.model = copyMatrix(model);
	updateBox(dbgModel.debugBox, model);
	updateDebugArrow(dbgModel.xArrow, dbgModel.debugBox, dbgModel.model);
	updateDebugArrow(dbgModel.yArrow, dbgModel.debugBox, dbgModel.model);
	updateDebugArrow(dbgModel.zArrow, dbgModel.debugBox, dbgModel.model);

	if (dbgModel.clickState == DebugClickState::NONE) {
		if (isLeftClickDown()) {
			Ray rayWorld = clickToRay(camera);
			if (isBoxInRayPath(dbgModel.debugBox, model, rayWorld)) {
				GLfloat distanceFromCamera = -1;

				if (isBoxInRayPath(dbgModel.xArrow.boundingBox, dbgModel.xArrow.model.model, rayWorld)) {
					dbgModel.clickState = DebugClickState::X_AXIS;
					distanceFromCamera = getDistanceFromCamera(dbgModel.xArrow.boundingBox, camera, dbgModel.xArrow.model.model);
				}
				
				if (isBoxInRayPath(dbgModel.yArrow.boundingBox, dbgModel.yArrow.model.model, rayWorld)) {
					GLfloat nextDistanceFromCamera = getDistanceFromCamera(dbgModel.yArrow.boundingBox, camera, dbgModel.yArrow.model.model);
					if (distanceFromCamera < 0 || nextDistanceFromCamera < distanceFromCamera) {
						dbgModel.clickState = DebugClickState::Y_AXIS;
						distanceFromCamera = nextDistanceFromCamera;
					}
				}
				
				if (isBoxInRayPath(dbgModel.zArrow.boundingBox, dbgModel.zArrow.model.model, rayWorld)) {
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

				if (dbgModel.clickState != DebugClickState::NONE) {
					dbgModel.focusToken = grabFocus();
				}
			}
		}
	} else if (!isLeftClickDown() && dbgModel.clickState != DebugClickState::NONE) {
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
		returnFocus(dbgModel.focusToken);
	} else {
		Ray rayToWorld = clickToRay(camera);
		Plane plane;
		plane.center =  fromVec4(mult(model, getCenter(dbgModel.debugBox)));
		Vector3f intersectPoint;

		switch (dbgModel.clickState) {
		case DebugClickState::X_AXIS:
			plane.normal = Vector3f{0, 0, 1};
			if (tryGetRayPointOfIntersection(plane, rayToWorld, intersectPoint)) {
				model = translateMatrixX(model, subtractVector(intersectPoint, plane.center).x);
			}
			break;
		case DebugClickState::Y_AXIS:
			plane.normal = Vector3f{0, 0, 1};
			if (tryGetRayPointOfIntersection(plane, rayToWorld, intersectPoint)) {
				model = translateMatrixY(model, subtractVector(intersectPoint, plane.center).y);
			}
			break;
		case DebugClickState::Z_AXIS:
			plane.normal = Vector3f{1, 0, 0};
			if (tryGetRayPointOfIntersection(plane, rayToWorld, intersectPoint)) {
				model = translateMatrixZ(model, subtractVector(intersectPoint, plane.center).z);
			}
			break;
		}
	}
}

void renderDebugModel(const DebugModel& dbgModel, const Matrix4x4f& model, const ModelUniformMapping& mapping) {
	renderDebugArrow(dbgModel.xArrow, mapping);
	renderDebugArrow(dbgModel.yArrow, mapping);
	renderDebugArrow(dbgModel.zArrow, mapping);
	renderBoxOutline(dbgModel.debugBox, model, mapping);
}

void freeDebug(DebugModel& dbgModel) {
	dbgModel.xArrow.model.free();
	dbgModel.yArrow.model.free();
	dbgModel.zArrow.model.free();
	
	if (dbgModel.debugBox.vao) glDeleteVertexArrays(1, &dbgModel.debugBox.vao);
	if (dbgModel.debugBox.vbo) glDeleteBuffers(1, &dbgModel.debugBox.vbo);
    if (dbgModel.debugBox.ebo) glDeleteBuffers(1, &dbgModel.debugBox.ebo);
}