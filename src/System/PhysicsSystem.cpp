#include "PhysicsSystem.h"
#include "../SystemEngine.h"

enum IntersectionResult {
	IntersectionResult_NONE,
	IntersectionResult_UP
};

const float gravity = 9.8; // m/s^2
const float small_delta = 0.2f;

void PhysicsSystem::initialize(SystemEngine* systemEngine) {
	mSystemEngine = systemEngine;
}

inline bool isBetween(float value, float start, float end) {
	return value >= start - small_delta && value <= end + small_delta;
}

inline IntersectionResult getIntersection(Box3D* first,
										  Matrix4x4f firstModel,
										  Box3D* second,
										  Matrix4x4f secondModel) {
	// @TODO Doesn't take into account rotation for now
	Vector4f firstLowerLeft = firstModel * first->lowerLeft;
	Vector4f firstUpperRight = firstModel * first->upperRight;

	Vector4f secondLowerLeft = secondModel * second->lowerLeft;
	Vector4f secondUpperRight = secondModel * second->upperRight;
	
	if (isBetween(firstLowerLeft.y, secondLowerLeft.y, secondUpperRight.y)) { // Y Plane intersect bottom
		if (isBetween(firstLowerLeft.x, secondLowerLeft.x, secondUpperRight.x)
			|| isBetween(firstUpperRight.x, secondLowerLeft.x, secondUpperRight.x)) { // X plane intersects

			if (isBetween(firstLowerLeft.z, secondLowerLeft.z, secondUpperRight.z)
				|| isBetween(firstUpperRight.z, secondLowerLeft.z, secondUpperRight.z)) { // Z plane intersects
				return IntersectionResult_UP;
			}
			
		}
	}

	return IntersectionResult_NONE;
}

void PhysicsSystem::update(float dt) {
	FOREACH_FIXED(mEntities) {
		Entity* entity = mSystemEngine->getEntity(value->mId);
		if (entity == nullptr) {
			continue;
		}

		if (value->isStatic) {
			continue;
		}
		
		value->applyForceTo(value->mass * gravity, { 0, -1, 0 });

		// Check forces against other entitieis
		auto outerValue = value;
		FOREACH_FIXED(mEntities) {
			// Steps:
			// 1) Check if bounding box intersect one another
			// 2) If they do, apply the force from one onto the force of the other, and vice versa
			// 3) If static, we can apply force, but it won't matter
			if (value->mId == outerValue->mId) {
				continue;
			}

			Entity* innerEntity = mSystemEngine->getEntity(value->mId);
			if (entity == nullptr) {
				continue;
			}
			
		    auto intersectionResult = getIntersection(&outerValue->mBox, entity->mModel, &value->mBox, innerEntity->mModel);

			switch (intersectionResult) {
			case IntersectionResult_UP: {
				outerValue->mLastIntersection = value->mId;
				outerValue->applyForceTo(value->mass * gravity, { 0, 1, 0 });
				break;
			}
			default: {
				break;
			}
			}
		}

		// At this point, we have added up all of the forces acting on the entity
	    

		Vector3f acceleration = {
			value->force.x / value->mass,
			value->force.y / value->mass,
			value->force.z / value->mass
		};

	    value->velocity = value->velocity + Vector3f {
			acceleration.x * dt,
			acceleration.y * dt,
			acceleration.z * dt
		};

		if (value->velocity.y >= 0) {
			value->velocity.y = 0; // @TODO: Assuming zero elasticity here
		}

		entity->mTranslation = entity->mTranslation + (value->velocity * dt);
		value->force = { 0, 0, 0 };
	}
}

void PhysicsEntity::applyForceTo(float inForce, Vector3f direction) {
	if (mass <= 0) {
		logger_error("Invalid mass value for entity: %u, %f", mId, mass);
		return;
	}

	direction = normalize(direction);
    force.x += direction.x * inForce;
	force.y += direction.y * inForce;
	force.z += direction.z * inForce;
}

void PhysicsSystem::free() {
	mEntities.clear();
}
