#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "ShaderUniformMapping.h"
#include <GL/glew.h>

void Model::update(float dt, Matrix4x4f model) {
    animationController.update(dt, bones, numBones, boneModels, inverseRootNode, rootNode);
	mModel = model;
}

void Model::render(const ModelUniformMapping& mapping, bool withMaterial) const {
    setShaderMat4(mapping.MODEL, mModel);
    setShaderBool(mapping.DISABLE_BONES, numBones <= 0);
    setShaderMat4Multiple(mapping.BONES, numBones, boneModels);
	for (int meshIdx = 0; meshIdx < numMeshes; meshIdx++) {
		meshes[meshIdx].render(mapping.materialUniformMapping, withMaterial);
    }
}

void Model::free() {
    if (meshes != nullptr) {
        for (int meshIdx = 0; meshIdx < numMeshes; meshIdx++) {
            meshes[meshIdx].free();
        }
        delete[] meshes;
		meshes = nullptr;
        numMeshes = 0;
    }

    if (bones != nullptr) {
        delete[] bones;
		bones = nullptr;
        numBones = 0;
    }

    if (rootNode) {
        rootNode->free();
        delete rootNode;
		rootNode = nullptr;
    }

    animationController.free();
}
