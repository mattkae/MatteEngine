#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "ShaderUniformMapping.h"
#include <GL/glew.h>

void Model::update(float dt) {
    animationController.update(dt, bones, numBones, boneModels, inverseRootNode, rootNode);
    Matrix4x4f translationMatrix = setTranslation(Matrix4x4f(), translation);
	Matrix4x4f rotationMatrix = rotation.normalize().toMatrix();
	Matrix4x4f scalingMatrix = setScale(Matrix4x4f(), scale);
    model = scalingMatrix * rotationMatrix * translationMatrix * defaultModel;
}

void Model::render(const ModelUniformMapping& mapping, bool withMaterial) const {
    setShaderMat4(mapping.MODEL, model);
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
        numMeshes = 0;
    }

    if (bones != nullptr) {
        delete[] bones;
        numBones = 0;
    }

    if (rootNode) {
        rootNode->free();
        delete rootNode;
    }

    animationController.free();
}