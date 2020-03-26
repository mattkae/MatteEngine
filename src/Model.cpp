#include "Model.h"
#include <GL/glew.h>

void Model::render(const Shader& shader, bool withMaterial) const {
    setShaderMat4(shader, "uModel", model);
	for (int meshIdx = 0; meshIdx < numMeshes; meshIdx++) {
		meshes[meshIdx].render(shader, withMaterial);
    }
}

void Model::free() {
    if (meshes) {
        for (int meshIdx = 0; meshIdx < numMeshes; meshIdx++) {
            meshes[meshIdx].free();
        }
    }
}