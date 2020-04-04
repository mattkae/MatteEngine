#include "LoadModel.h"
#include "BinarySerializer.h"

void LoadModel::writeLoadModel(BinarySerializer& serializer) {
	calculateBoundingBox();
	serializer.writeVec4(lowerLeftBoundingBoxCorner);
	serializer.writeVec4(upperRightBoundingBoxCorner);

	serializer.writeInt32(meshes.size());
	for (LoadMesh& mesh: meshes) {
		mesh.write(serializer);
	}

	serializer.writeUint32((unsigned int)bones.size());
	for (LoadBone& bone : bones) {
		bone.write(serializer);
	}

	serializer.writeUint32((unsigned int)animations.size());
	for (Animation& animation : animations) {
		animation.write(serializer);
	}
}

void LoadModel::readLoadModel(BinarySerializer& serializer) {
	lowerLeftBoundingBoxCorner = serializer.readVec4();
	upperRightBoundingBoxCorner = serializer.readVec4();

	int numMeshes = serializer.readInt32();
	meshes.resize(numMeshes);
	for (LoadMesh& mesh: meshes) {
		mesh.read(serializer);
	}

	unsigned int numBones = serializer.readUint32();
	bones.resize(numBones);
	for (LoadBone& bone: bones) {
		bone.read(serializer);
	}

	unsigned int numAnimations = serializer.readUint32();
	animations.resize(numAnimations);
	for (Animation& animation: animations) {
		animation.read(serializer);
	}
}

void LoadModel::calculateBoundingBox() {
	bool hasBoxBeenSet = false;
	for (const LoadMesh& mesh: meshes) {
		for (auto& vertex : mesh.vertices) {
			if (!hasBoxBeenSet) {
				lowerLeftBoundingBoxCorner.x = vertex.position.x;
				lowerLeftBoundingBoxCorner.y = vertex.position.y;
				lowerLeftBoundingBoxCorner.z = vertex.position.z;
				upperRightBoundingBoxCorner.x = vertex.position.x;
				upperRightBoundingBoxCorner.y = vertex.position.y;
				upperRightBoundingBoxCorner.z = vertex.position.z;
				hasBoxBeenSet = true;
			}
			
			if (vertex.position.x < lowerLeftBoundingBoxCorner.x) {
				lowerLeftBoundingBoxCorner.x = vertex.position.x;
			}
			if (vertex.position.y < lowerLeftBoundingBoxCorner.y) {
				lowerLeftBoundingBoxCorner.y = vertex.position.y;
			}
			if (vertex.position.z < lowerLeftBoundingBoxCorner.z) {
				lowerLeftBoundingBoxCorner.z = vertex.position.z;
			}

			if (vertex.position.x > upperRightBoundingBoxCorner.x) {
				upperRightBoundingBoxCorner.x = vertex.position.x;
			}
			if (vertex.position.y > upperRightBoundingBoxCorner.y) {
				upperRightBoundingBoxCorner.y = vertex.position.y;
			}
			if (vertex.position.z > upperRightBoundingBoxCorner.z) {
				upperRightBoundingBoxCorner.z = vertex.position.z;
			}
		}
	}
}

void LoadMesh::write(BinarySerializer& serializer) {
	serializer.writeInt32(vertices.size());
	for (LoadVertex& vertex: vertices) {
		vertex.write(serializer);
	}

	serializer.writeInt32(indices.size());
	for (GLint index: indices) {
		serializer.writeInt32(index);
	}

	material.write(serializer);
}

void LoadMesh::read(BinarySerializer& serializer) {
	vertices.resize(serializer.readInt32());
	for (LoadVertex& vertex: vertices) {
		vertex.read(serializer);
	}

	int indicesSize = serializer.readInt32();
	indices.resize(indicesSize);
	for (int indexIdx = 0; indexIdx < indicesSize; indexIdx++) {
		indices[indexIdx] = serializer.readInt32();
	}

	material.read(serializer);
}

void LoadVertex::write(BinarySerializer& serializer) {
	serializer.writeVec3(position);
	serializer.writeVec3(normal);
	serializer.writeVec2(texCoords);
	serializer.writeUint32(boneInfoList.size());
	for (LoadVertexBoneData& boneData : boneInfoList) {
		serializer.writeUint32(boneData.boneIndex);
		serializer.writeFloat32(boneData.weight);
	}
}

void LoadVertex::read(BinarySerializer& serializer) {
	position = serializer.readVec3();
	normal = serializer.readVec3();
	texCoords = serializer.readVec2();

	boneInfoList.resize(serializer.readUint32());
	for (LoadVertexBoneData& boneData : boneInfoList) {
		boneData.boneIndex = serializer.readUint32();
		boneData.weight = serializer.readFloat32();
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
	serializer.writeUint32(nodeUniqueId);
	serializer.writeUint32(parentNodeUniqueId);
}

void LoadBone::read(BinarySerializer& serializer) {
	offsetMatrix = serializer.readMat4x4();
	nodeUniqueId = serializer.readUint32();
	parentNodeUniqueId = serializer.readUint32();
}