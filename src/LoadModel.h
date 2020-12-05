#pragma once
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix4x4f.h"
#include "AnimationController.h"
#include "List.h"
#include "MyString.h"
#include <GL/glew.h>

class BinarySerializer;

struct LoadMaterial {
	Vector3f emissive = { 0.f, 0.f, 0.f };
    Vector3f ambient = { 0.f, 0.f, 0.f };
    Vector3f diffuse = { 1.f, 1.f, 1.f };
    Vector3f specular = { 0.f, 0.f, 0.f };
    Vector3f diffuseProperty = { 1.f, 1.f, 1.f };
    Vector3f specularProperty = { 1.f, 1.f, 1.f };
    GLfloat specularComponent = 20.f;
    GLfloat transparency = 1.0;

    int diffuseUniqueTextureId = -1;
    int specularUniqueTextureId = -1;
    int ambientUniqueTextureId = -1;

    void write(BinarySerializer& serializer);
    void read(BinarySerializer& serializer);
};

struct LoadVertexBoneData {
    unsigned int boneIndex;
    GLfloat weight;
};

struct LoadVertex {
    Vector3f position;
    Vector3f normal;
    Vector3f tangent;
    Vector3f bitangent;
    Vector2f texCoords;
    List<LoadVertexBoneData> boneInfoList;

    void write(BinarySerializer& serializer);
    void read(BinarySerializer& serializer);
};

struct LoadMesh {
    List<LoadVertex> vertices;
    List<GLint> indices;
    LoadMaterial material;

    void write(BinarySerializer& serializer);
    void read(BinarySerializer& serializer);
};

struct LoadBone {
    Matrix4x4f offsetMatrix;
    String identifier;
    void write(BinarySerializer& serializer);
    void read(BinarySerializer& serializer);
};

struct LoadBoneNode {
    int boneIndex = -1;
    List<LoadBoneNode> children;
    Matrix4x4f nodeTransform;

    void write(BinarySerializer& serializer);
    void read(BinarySerializer& serializer);
};

struct LoadModel {
    char* modelPath;
    Vector4f lowerLeftBoundingBoxCorner;
    Vector4f upperRightBoundingBoxCorner;
    Matrix4x4f inverseRootNode;

    LoadBoneNode rootNode;
    List<LoadBone> bones;
    List<LoadMesh> meshes;
    List<Animation> animations;

    void writeLoadModel(BinarySerializer& serializer);
    void readLoadModel(BinarySerializer& serializer);
    void calculateBoundingBox();
};
