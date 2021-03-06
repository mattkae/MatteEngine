#pragma once
#include "LoadModel.h"
#include "Constants.h"

struct Vertex {
    Vector3f position;
    Vector3f normal;
    Vector3f tangent;
    Vector3f bitangent;
    Vector2f texCoords;
    GLfloat boneWeights[4] = { 0, 0, 0, 0 };
    GLint boneIndices[4] = { 0, 0, 0, 0 };

    void initialize(const LoadVertex& vertex) {
        position = vertex.position;
        normal = vertex.normal;
        tangent = vertex.tangent;
        bitangent = vertex.bitangent;
        texCoords = vertex.texCoords;

        size_t max = static_cast<size_t>(fmin(vertex.boneInfoList.numElements, 4));
        for (size_t index = 0; index < max; index++) {
            boneWeights[index] = vertex.boneInfoList[index].weight;
            boneIndices[index] = (GLint)vertex.boneInfoList[index].boneIndex;
        }
    }
};
