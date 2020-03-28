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

struct Model {
    Matrix4x4f model;

    Bone* bones;
    unsigned int numBones;

    Mesh* meshes;
    int numMeshes;

    AnimationController animationController;

    void update(float dt);
    void render(const Shader& shader, bool withMaterial = true) const;
    void free();
};

#endif
