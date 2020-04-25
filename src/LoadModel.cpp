#include "LoadModel.h"
#include "BinarySerializer.h"
#include <algorithm>

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

	rootNode.write(serializer);

	serializer.writeUint32((unsigned int)animations.size());
	for (Animation& animation : animations) {
		animation.write(serializer);
	}
	serializer.writeMat4x4(inverseRootNode);
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

	rootNode.read(serializer);

	unsigned int numAnimations = serializer.readUint32();
	animations.resize(numAnimations);
	for (Animation& animation: animations) {
		animation.read(serializer);
	}
	inverseRootNode = serializer.readMat4x4();
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

bool compareLoadVertexBoneData(LoadVertexBoneData first, LoadVertexBoneData second) {
	return first.weight > second.weight;
}

void LoadVertex::write(BinarySerializer& serializer) {
	serializer.writeVec3(position);
	serializer.writeVec3(normal);
	serializer.writeVec2(texCoords);
	serializer.writeUint32(boneInfoList.size());

	std::sort(boneInfoList.begin(), boneInfoList.end(), compareLoadVertexBoneData);

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
}

void LoadBone::read(BinarySerializer& serializer) {
	offsetMatrix = serializer.readMat4x4();
}

void LoadBoneNode::write(BinarySerializer& serializer) {
	serializer.writeUint32(boneIndex);
	serializer.writeMat4x4(nodeTransform);
	serializer.writeUint32(children.size());
	for (LoadBoneNode& child: children) {
		child.write(serializer);
	}
}

void LoadBoneNode::read(BinarySerializer& serializer) {
	boneIndex = serializer.readUint32();
	nodeTransform = serializer.readMat4x4();
	children.resize(serializer.readUint32());
	for (LoadBoneNode& child: children) {
		child.read(serializer);
	}
}