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
struct ModelUniformMapping;

constexpr unsigned int MAX_BONES = 64;

// @TODO Break this up into ModelRender and ModelUpdate
struct Model {
	Matrix4x4f mModel;
    Matrix4x4f inverseRootNode;

    Matrix4x4f boneModels[MAX_BONES];
    Bone* bones = nullptr;
    unsigned int numBones = 0;
    BoneTreeNode* rootNode = nullptr;

    Mesh* meshes = nullptr;
    int numMeshes = 0;

    AnimationController animationController;

    void update(float dt, Matrix4x4f model);
    void render(const ModelUniformMapping& mapping, bool withMaterial = true) const;
    void free();
};

#endif
