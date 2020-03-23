#pragma once
#include "Vector3f.h"
#include "Vertex.h"
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

struct LoadMesh {
    std::vector<Vertex> vertices;
    std::vector<GLint> indices;
    LoadMaterial material;
};

struct LoadModel {
    std::vector<LoadMesh> meshes;
    std::string modelPath;

    void writeLoadModel(BinarySerializer& serializer);
    void readLoadModel(BinarySerializer& serializer);
};
