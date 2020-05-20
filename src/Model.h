#ifndef MODEL_H
#define MODEL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Shader.h"
#include "Quaternion.h"
#include <string>
#include "Matrix4x4f.h"
#include "AnimationController.h"

struct Bone;
struct BoneTreeNode;
struct Mesh;
struct LoadModel;

constexpr unsigned int MAX_BONES = 64;

// @TODO Break this up into ModelRender and ModelUpdate
struct Model {
    Vector3f translation;
    Vector3f scale = Vector3f{ 1.f, 1.f, 1.f };
    Quaternion rotation;
    Matrix4x4f model;
    Matrix4x4f inverseRootNode;

    Matrix4x4f boneModels[MAX_BONES];
    Bone* bones = nullptr;
    unsigned int numBones = 0;
    BoneTreeNode* rootNode;

    Mesh* meshes = nullptr;
    int numMeshes = 0;

    AnimationController animationController;

    void update(float dt);
    void render(const Shader& shader, bool withMaterial = true) const;
    void free();
};

#endif
