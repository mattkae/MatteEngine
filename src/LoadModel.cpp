#include "LoadModel.h"
#include "BinarySerializer.h"

void LoadModel::writeLoadModel(BinarySerializer& serializer) {
	calculateBoundingBox();
	serializer.writeVec4(lowerLeftBoundingBoxCorner);
	serializer.writeVec4(upperRightBoundingBoxCorner);

	serializer.writeInt32(meshes.numElements);
	FOREACH(meshes) {
		value->write(serializer);
	}

	serializer.writeUint32((unsigned int)bones.numElements);
	FOREACH(bones) {
		value->write(serializer);
	}

	rootNode.write(serializer);

	serializer.writeUint32((unsigned int)animations.numElements);
	FOREACH(animations) {
		value->write(serializer);
	}
	serializer.writeMat4x4(inverseRootNode);
}

void LoadModel::readLoadModel(BinarySerializer& serializer) {
	lowerLeftBoundingBoxCorner = serializer.readVec4();
	upperRightBoundingBoxCorner = serializer.readVec4();

	int numMeshes = serializer.readInt32();
	meshes.allocate(numMeshes);
	meshes.growDynamically = false;
	for (int idx = 0; idx < numMeshes; idx++) {
		LoadMesh newMesh;
		newMesh.read(serializer);
		meshes.add(&newMesh);
	}

	unsigned int numBones = serializer.readUint32();
	bones.allocate(numBones);
	bones.growDynamically = true;
	for (int idx = 0; idx < numBones; idx++) {
		LoadBone newBone;
		newBone.read(serializer);
		bones.add(newBone);
	}

	rootNode.read(serializer);

	unsigned int numAnimations = serializer.readUint32();
	animations.allocate(numAnimations);
	animations.growDynamically = false;
	for (int idx = 0; idx < numAnimations; idx++) {
		Animation animation;
		animation.read(serializer);
		animations.add(animation);
	}
	inverseRootNode = serializer.readMat4x4();
}

void LoadModel::calculateBoundingBox() {
	bool hasBoxBeenSet = false;
	FOREACH(meshes) {
		List<LoadVertex> vertices = value->vertices;
		FOREACH (vertices) {
			if (!hasBoxBeenSet) {
				lowerLeftBoundingBoxCorner.x = value->position.x;
				lowerLeftBoundingBoxCorner.y = value->position.y;
				lowerLeftBoundingBoxCorner.z = value->position.z;
				upperRightBoundingBoxCorner.x = value->position.x;
				upperRightBoundingBoxCorner.y = value->position.y;
				upperRightBoundingBoxCorner.z = value->position.z;
				hasBoxBeenSet = true;
			}
			
			if (value->position.x < lowerLeftBoundingBoxCorner.x) {
				lowerLeftBoundingBoxCorner.x = value->position.x;
			}
			if (value->position.y < lowerLeftBoundingBoxCorner.y) {
				lowerLeftBoundingBoxCorner.y = value->position.y;
			}
			if (value->position.z < lowerLeftBoundingBoxCorner.z) {
				lowerLeftBoundingBoxCorner.z = value->position.z;
			}

			if (value->position.x > upperRightBoundingBoxCorner.x) {
				upperRightBoundingBoxCorner.x = value->position.x;
			}
			if (value->position.y > upperRightBoundingBoxCorner.y) {
				upperRightBoundingBoxCorner.y = value->position.y;
			}
			if (value->position.z > upperRightBoundingBoxCorner.z) {
				upperRightBoundingBoxCorner.z = value->position.z;
			}
		}
	}
}

void LoadMesh::write(BinarySerializer& serializer) {
	serializer.writeInt32(vertices.numElements);
	FOREACH(vertices) {
		value->write(serializer);
	}

	serializer.writeInt32(indices.numElements);
	FOREACH(indices) {
		serializer.writeInt32(*value);
	}

	material.write(serializer);
}

void LoadMesh::read(BinarySerializer& serializer) {
	vertices.allocate(serializer.readInt32());
	vertices.growDynamically = false;
	for (int idx = 0; idx < vertices.capacity; idx++) {
		LoadVertex vertex;
		vertex.read(serializer);
		vertices.add(&vertex);
	}

	int indicesSize = serializer.readInt32();
	indices.allocate(indicesSize);
	indices.growDynamically = false;
	for (int indexIdx = 0; indexIdx < indicesSize; indexIdx++) {
		int index = serializer.readInt32();
		indices.add(&index);
	}

	material.read(serializer);
}

int compareLoadVertexBoneData(LoadVertexBoneData* first, LoadVertexBoneData* second) {
	return first->weight - second->weight;
}

void LoadVertex::write(BinarySerializer& serializer) {
	serializer.writeVec3(position);
	serializer.writeVec3(normal);
	serializer.writeVec3(tangent);
	serializer.writeVec3(bitangent);
	serializer.writeVec2(texCoords);
	serializer.writeUint32(boneInfoList.numElements);

	boneInfoList.binarySort(compareLoadVertexBoneData);

	FOREACH(boneInfoList) {
		serializer.writeUint32(value->boneIndex);
		serializer.writeFloat32(value->weight);
	}
}

void LoadVertex::read(BinarySerializer& serializer) {
	position = serializer.readVec3();
	normal = serializer.readVec3();
	tangent = serializer.readVec3();
	bitangent = serializer.readVec3();
	texCoords = serializer.readVec2();

	unsigned int numBones = serializer.readUint32();
	boneInfoList.allocate(numBones);
	boneInfoList.growDynamically = false;
	for (int idx = 0; idx < numBones; idx++) {
		LoadVertexBoneData boneData;
		boneData.boneIndex = serializer.readUint32();
		boneData.weight = serializer.readFloat32();
		boneInfoList.add(&boneData);
	}
}

void LoadMaterial::write(BinarySerializer& serializer) {
	serializer.writeVec3(emissive);
	serializer.writeVec3(ambient);
	serializer.writeVec3(diffuse);
	serializer.writeVec3(specular);
	serializer.writeFloat32(transparency);
	serializer.writeInt32(diffuseUniqueTextureId);
	serializer.writeInt32(specularUniqueTextureId);
	serializer.writeInt32(ambientUniqueTextureId);
}

void LoadMaterial::read(BinarySerializer& serializer) {
	emissive = serializer.readVec3();
	ambient = serializer.readVec3();
	diffuse = serializer.readVec3();
	specular = serializer.readVec3();
	transparency = serializer.readFloat32();
	diffuseUniqueTextureId = serializer.readInt32();
	specularUniqueTextureId = serializer.readInt32();
	ambientUniqueTextureId = serializer.readInt32();
}

void LoadBone::write(BinarySerializer& serializer) {
	serializer.writeMat4x4(offsetMatrix);
}

void LoadBone::read(BinarySerializer& serializer) {
	offsetMatrix = serializer.readMat4x4();
}

void LoadBoneNode::write(BinarySerializer& serializer) {
	serializer.writeUint32(boneIndex);
	serializer.writeMat4x4(nodeTransform);
	serializer.writeUint32(children.numElements);
	FOREACH(children) {
		value->write(serializer);
	}
}

void LoadBoneNode::read(BinarySerializer& serializer) {
	boneIndex = serializer.readUint32();
	nodeTransform = serializer.readMat4x4();
	unsigned int numChildren = serializer.readUint32();
	children.allocate(numChildren);
	for (unsigned int idx = 0; idx < numChildren; idx++) {
		LoadBoneNode child;
		child.read(serializer);
		children.add(&child);
	}
}