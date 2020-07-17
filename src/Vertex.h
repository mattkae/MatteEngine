#pragma once
#include "LoadModel.h"
#include "Constants.h"

struct Vertex {
    Vector3f position;
    Vector3f normal;
    Vector3f tangent;
    Vector3f bitangent;
    Vector2f texCoords;
    GLfloat textureWeights[Constants::Rendering::MAX_TEXTURES_PER_MESH] = { 1, 0, 0 };
    GLfloat boneWeights[4] = { 0, 0, 0, 0 };
    GLint boneIndices[4] = { 0, 0, 0, 0 };

    void initialize(const LoadVertex& vertex) {
        position = vertex.position;
        normal = vertex.normal;
        tangent = vertex.tangent;
        bitangent = vertex.bitangent;
        texCoords = vertex.texCoords;

        size_t max = fmin(vertex.boneInfoList.size(), 4);
        for (size_t index = 0; index < max; index++) {
            boneWeights[index] = vertex.boneInfoList[index].weight;
            boneIndices[index] = (GLint)vertex.boneInfoList[index].boneIndex;
        }
    }
};