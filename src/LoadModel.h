#pragma once
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Vertex.h"
#include "Matrix4x4f.h"
#include <GL/glew.h>
#include <vector>

class BinarySerializer;

struct LoadMaterial {
	Vector3f emissive = { 0.f, 0.f, 0.f };
    Vector3f ambient = { 1.f, 1.f, 1.f };
    Vector3f diffuse = { 1.f, 1.f, 1.f };
    Vector3f specular = { 1.f, 1.f, 1.f };
    Vector3f diffuseProperty = { 1.f, 1.f, 1.f };
    Vector3f specularProperty = { 1.f, 1.f, 1.f };
    GLfloat specularComponent = 20.f;
    GLfloat transparency = 1.0;

    int diffuseUniqueTextureId = -1;
    int specularUniqueTextureId = -1;
    int ambientUniqueTextureId = -1;
};

struct LoadVertexBoneData {
    unsigned int boneIndex;
    GLfloat weight;
};

struct LoadVertex {
    Vector3f position;
    Vector3f normal;
    Vector2f texCoords;
    std::vector<LoadVertexBoneData> boneInfoList;
};

struct LoadMesh {
    unsigned int nodeUniqueId;
    std::vector<LoadVertex> vertices;
    std::vector<GLint> indices;
    LoadMaterial material;
};

struct LoadBone {
    Matrix4x4f offsetMatrix;
    unsigned int nodeUniqueId;
};

struct LoadModel {
    std::vector<LoadBone> bones;
    std::vector<LoadMesh> meshes;
    std::string modelPath;
    Vector4f lowerLeftBoundingBoxCorner;
    Vector4f upperRightBoundingBoxCorner;

    void writeLoadModel(BinarySerializer& serializer);
    void readLoadModel(BinarySerializer& serializer);
    void calculateBoundingBox();
};
