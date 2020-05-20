#include "Model.h"
#include "Mesh.h"
#include "Bone.h"
#include <GL/glew.h>

void Model::update(float dt) {
    animationController.update(dt, bones, numBones, boneModels, inverseRootNode, rootNode);
    Matrix4x4f translationMatrix = setTranslation(Matrix4x4f(), translation);
	Matrix4x4f rotationMatrix = rotation.normalize().toMatrix();
	Matrix4x4f scalingMatrix = setScale(Matrix4x4f(), scale);
    model = scalingMatrix * rotationMatrix * translationMatrix;
}

void Model::render(const Shader& shader, bool withMaterial) const {
    setShaderMat4(shader, "uModel", model);
    setShaderMat4Multiple(shader, "uBones", numBones, boneModels);
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

    if (rootNode) {
        rootNode->free();
        delete rootNode;
    }

    animationController.free();
}