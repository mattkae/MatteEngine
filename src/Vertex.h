#pragma once
#include "LoadModel.h"

struct Vertex {
    Vector3f position;
    Vector3f normal;
    Vector3f texCoords;
    GLfloat boneWeights[4] = { 0, 0, 0, 0 };
    GLint boneIndices[4] = { 0, 0, 0, 0 };

    void initialize(const LoadVertex& vertex) {
        position = vertex.position;
        normal = vertex.normal;
        texCoords = getVec3(vertex.texCoords.x, vertex.texCoords.y, 0);

        size_t max = fmin(vertex.boneInfoList.size(), 4);
        for (size_t index = 0; index < max; index++) {
            boneWeights[index] = vertex.boneInfoList[index].weight;
            boneIndices[index] = (GLint)vertex.boneInfoList[index].boneIndex;
        }
    }
};