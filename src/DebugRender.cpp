#include "DebugRender.h"
#include "Input.h"
#include "Ray.h"
#include "Plane.h"
#include "ModelLoader.h"
#include "Mesh.h"
#include "Box.h"

inline void changeArrowColor(DebugArrow& arrow, Vector3f color) {
	color = color;
	for (int meshIdx = 0; meshIdx < arrow.model.numMeshes; meshIdx++) {
		arrow.model.meshes[meshIdx].material.emissive = color;
		arrow.model.meshes[meshIdx].material.ambient = color;
		arrow.model.meshes[meshIdx].material.diffuse = color;
	}
}

inline void initArrow(DebugArrow& arrow) {
	/*ModelLoader::ModelLoadResult retval = ModelLoader::loadSerializedModel((char*)"assets/models/Arrow/simple_mattl");
	model = retval.model;
	boundingBox = retval.box;
	changeArrowColor(arrow, color);*/
}

void DebugArrow::update(Box3D& box, const Matrix4x4f& inModel) {
	model.model = getIdentity();
	if (model.numMeshes == 0) {
		initArrow(*this);
	}

	// Rotate and scale per its direction
	Vector4f upperRight = boundingBox.upperRight;
	Vector4f lowerLeft = boundingBox.lowerLeft;
	Vector3f translation = scale(Vector3f{0, 1, 0}, abs(upperRight.y - lowerLeft.y) / 2.f);

	model.model = translateMatrix(model.model, translation);
	model.model = rotate(model.model, rotation.x, rotation.y, rotation.z);
	model.model = mult(model.model, inModel);

	// Center inside of the bounding box
	Vector4f boxUpperRight = mult(inModel, box.upperRight);
	Vector4f boxLowerLeft = mult(inModel, box.lowerLeft);
	Vector3f centerOfBox = Vector3f{ 0, fabsf(boxUpperRight.y - boxLowerLeft.y) / 2.f, 0 };
	model.model = translateMatrix(model.model, centerOfBox);
}

void DebugArrow::render(const ModelUniformMapping& mapping) const {
	model.render(mapping);
}

void DebugModel::update(Matrix4x4f& model, const Camera& camera) {
	model = copyMatrix(model);
	updateBox(debugBox);
	xArrow.update(debugBox, model);
	yArrow.update(debugBox, model);
	zArrow.update(debugBox, model);

	if (clickState == DebugClickState::NONE) {
		if (isLeftClickDown()) {
			Ray rayWorld = clickToRay(camera);
			if (isBoxInRayPath(debugBox, model, rayWorld)) {
				GLfloat distanceFromCamera = -1;

				if (isBoxInRayPath(xArrow.boundingBox, xArrow.model.model, rayWorld)) {
					clickState = DebugClickState::X_AXIS;
					distanceFromCamera = getDistanceFromCamera(xArrow.boundingBox, camera, xArrow.model.model);
				}
				
				if (isBoxInRayPath(yArrow.boundingBox, yArrow.model.model, rayWorld)) {
					GLfloat nextDistanceFromCamera = getDistanceFromCamera(yArrow.boundingBox, camera, yArrow.model.model);
					if (distanceFromCamera < 0 || nextDistanceFromCamera < distanceFromCamera) {
						clickState = DebugClickState::Y_AXIS;
						distanceFromCamera = nextDistanceFromCamera;
					}
				}
				
				if (isBoxInRayPath(zArrow.boundingBox, zArrow.model.model, rayWorld)) {
					GLfloat nextDistanceFromCamera = getDistanceFromCamera(zArrow.boundingBox, camera, zArrow.model.model);
					if (distanceFromCamera < 0 || nextDistanceFromCamera < distanceFromCamera) {
						clickState = DebugClickState::Z_AXIS;
					}
				}

				switch (clickState) {
				case DebugClickState::X_AXIS:
					changeArrowColor(xArrow, Vector3f{1, 1, 0});
					break;
				case DebugClickState::Y_AXIS:
					changeArrowColor(yArrow, Vector3f{1, 1, 0});
					break;
				case DebugClickState::Z_AXIS:
					changeArrowColor(zArrow, Vector3f{1, 1, 0});
					break;
				}

				if (clickState != DebugClickState::NONE) {
					focusToken = grabFocus();
				}
			}
		}
	} else if (!isLeftClickDown() && clickState != DebugClickState::NONE) {
		switch (clickState) {
		case DebugClickState::X_AXIS:
			changeArrowColor(xArrow, Vector3f{1, 0, 0});
			break;
		case DebugClickState::Y_AXIS:
			changeArrowColor(yArrow, Vector3f{0, 1, 0});
			break;
		case DebugClickState::Z_AXIS:
			changeArrowColor(zArrow, Vector3f{0, 0, 1});
			break;
		}

		clickState = DebugClickState::NONE;
		returnFocus(focusToken);
	} else {
		Ray rayToWorld = clickToRay(camera);
		Plane plane;
		plane.center =  fromVec4(mult(model, getCenter(debugBox)));
		Vector3f intersectPoint;

		switch (clickState) {
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

void DebugModel::render( const Matrix4x4f& model, const ModelUniformMapping& mapping) const {
	xArrow.render(mapping);
	yArrow.render(mapping);
	zArrow.render(mapping);
	renderBoxOutline(debugBox, model, mapping, true);
}

void DebugModel::free() {
	xArrow.model.free();
	yArrow.model.free();
	zArrow.model.free();
	/*
	if (debugBox.vao) glDeleteVertexArrays(1, &debugBox.vao);
	if (debugBox.vbo) glDeleteBuffers(1, &debugBox.vbo);
    if (debugBox.ebo) glDeleteBuffers(1, &debugBox.ebo);
	*/
}
