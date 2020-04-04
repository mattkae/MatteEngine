#ifndef MODEL_H
#define MODEL_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Shader.h"
#include <string>
#include <vector>
#include "Matrix4x4f.h"
#include "AnimationController.h"

struct Bone;
struct Mesh;

constexpr unsigned int MAX_BONES = 16;

struct Model {
    Matrix4x4f model;

    Matrix4x4f boneModels[MAX_BONES];
    Bone* bones = nullptr;
    unsigned int numBones = 0;

    Mesh* meshes = nullptr;
    int numMeshes = 0;

    AnimationController animationController;

    void initialize();
    void update(float dt);
    void render(const Shader& shader, bool withMaterial = true) const;
    void free();
};

#endif
