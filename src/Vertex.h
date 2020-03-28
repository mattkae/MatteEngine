#ifndef VERTEX_H
#define VERTEX_H
#include "Vector2f.h"
#include "Vector3f.h"

struct VertexBone {
    unsigned int boneIndex;
    GLfloat weight;
};

struct Vertex {
    Vector3f position;
    Vector3f normal;
    Vector2f texCoords;

    unsigned int numBones;
    VertexBone* boneInfoList;
};

#endif
