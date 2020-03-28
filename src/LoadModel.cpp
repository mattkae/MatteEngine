#include "LoadModel.h"
#include "BinarySerializer.h"
void LoadModel::writeLoadModel(BinarySerializer& serializer) {
	calculateBoundingBox();

	serializer.writeInt32(meshes.size());
	for (LoadMesh& mesh: meshes) {
		serializer.writeInt32(mesh.vertices.size());
		for (LoadVertex& vertex: mesh.vertices) {
			serializer.writeVec3(vertex.position);
			serializer.writeVec3(vertex.normal);
			serializer.writeVec2(vertex.texCoords);
		}

		serializer.writeInt32(mesh.indices.size());
		for (GLint index: mesh.indices) {
			serializer.writeInt32(index);
		}

		serializer.writeVec3(mesh.material.emissive);
		serializer.writeVec3(mesh.material.ambient);
		serializer.writeVec3(mesh.material.diffuse);
		serializer.writeVec3(mesh.material.specular);
		serializer.writeFloat32(mesh.material.transparency);
		serializer.writeInt32(mesh.material.diffuseUniqueTextureId);
		serializer.writeInt32(mesh.material.specularUniqueTextureId);
		serializer.writeInt32(mesh.material.ambientUniqueTextureId);
	}

	serializer.writeVec4(lowerLeftBoundingBoxCorner);
	serializer.writeVec4(upperRightBoundingBoxCorner);
}

void LoadModel::readLoadModel(BinarySerializer& serializer) {
	int numMeshes = serializer.readInt32();
	meshes.resize(numMeshes);

	for (int meshIdx = 0; meshIdx < numMeshes; meshIdx++) {
		LoadMesh& mesh = meshes[meshIdx];

		int numVertices = serializer.readInt32();
		mesh.vertices.resize(numVertices);
		for (int vertexIdx = 0; vertexIdx < numVertices; vertexIdx++) {
			mesh.vertices[vertexIdx].position = serializer.readVec3();
			mesh.vertices[vertexIdx].normal = serializer.readVec3();
			mesh.vertices[vertexIdx].texCoords = serializer.readVec2();
		}

		int indicesSize = serializer.readInt32();
		mesh.indices.resize(indicesSize);
		for (int indexIdx = 0; indexIdx < indicesSize; indexIdx++) {
			mesh.indices[indexIdx] = serializer.readInt32();
		}

		mesh.material.emissive = serializer.readVec3();
		mesh.material.ambient = serializer.readVec3();
		mesh.material.diffuse = serializer.readVec3();
		mesh.material.specular = serializer.readVec3();
		mesh.material.transparency = serializer.readFloat32();
		mesh.material.diffuseUniqueTextureId = serializer.readInt32();
		mesh.material.specularUniqueTextureId = serializer.readInt32();
		mesh.material.ambientUniqueTextureId = serializer.readInt32();
	}

	lowerLeftBoundingBoxCorner = serializer.readVec4();
	upperRightBoundingBoxCorner = serializer.readVec4();
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