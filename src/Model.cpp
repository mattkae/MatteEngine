#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include <GL/glew.h>

void Model::initialize() {

}

void Model::update(float dt) {
    animationController.update(dt, bones, numBones);
}

void Model::render(const Shader& shader, bool withMaterial) const {
    setShaderMat4(shader, "uModel", model);
	for (int meshIdx = 0; meshIdx < numMeshes; meshIdx++) {
		meshes[meshIdx].render(shader, withMaterial);
    }
}

void Model::free() {
    if (meshes != nullptr) {
        for (int meshIdx = 0; meshIdx < numMeshes; meshIdx++) {
            meshes[meshIdx].free();
        }
        delete[] meshes;
        numMeshes = 0;
    }

    if (bones != nullptr) {
        delete[] bones;
        numBones = 0;
    }

    if (animationController.animationList) {
        // @TODO: Free animation controller
    }
}