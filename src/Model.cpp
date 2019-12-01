#include "Model.h"
#include "GlmUtility.h"
#include "ObjModel.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

inline void trySetBoxBounds(Box& box, Mesh& mesh, bool& hasBoxBeenSet) {
	for (auto& vertex : mesh.vertices) {
		if (!hasBoxBeenSet) {
			box.lowerLeft.x = vertex.position.x;
			box.lowerLeft.y = vertex.position.y;
			box.lowerLeft.z = vertex.position.z;
			box.upperRight.x = vertex.position.x;
			box.upperRight.y = vertex.position.y;
			box.upperRight.z = vertex.position.z;
			hasBoxBeenSet = true;
		}
			
		if (vertex.position.x < box.lowerLeft.x) {
			box.lowerLeft.x = vertex.position.x;
		}
		if (vertex.position.y < box.lowerLeft.y) {
			box.lowerLeft.y = vertex.position.y;
		}
		if (vertex.position.z < box.lowerLeft.z) {
			box.lowerLeft.z = vertex.position.z;
		}

		if (vertex.position.x > box.upperRight.x) {
			box.upperRight.x = vertex.position.x;
		}
		if (vertex.position.y > box.upperRight.y) {
			box.upperRight.y = vertex.position.y;
		}
		if (vertex.position.z > box.upperRight.z) {
			box.upperRight.z = vertex.position.z;
		}
	}
}

void initializeModel(Model& model, Box& box) {
	bool hasBoxBeenSet = false;

    for (auto& mesh : model.meshes) {
        initializeMesh(mesh);
		trySetBoxBounds(box, mesh, hasBoxBeenSet);
    }
}

void freeModel(Model& model) {
    for (auto& mesh : model.meshes) {
        freeMesh(mesh);
    }

    model.meshes.clear();
}

void renderModel(const Model& model, const Shader& shader, bool withMaterial) {
    setShaderMat4(shader, "uModel", model.model);
    for (auto& mesh : model.meshes) {
		renderMesh(mesh, shader, withMaterial);
    }
}
