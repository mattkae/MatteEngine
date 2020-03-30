
#include "Mesh.h"
#include "LoadModel.h"

using namespace std;

void Mesh::initialize(LoadMesh& loadMesh, List* list) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, loadMesh.vertices.size() * sizeof(LoadVertex), &loadMesh.vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadMesh.indices.size() * sizeof(GLuint), &loadMesh.indices[0], GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(LoadVertex), (GLvoid *)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(LoadVertex), (GLvoid *)offsetof(LoadVertex, normal));

    // Texture Coordinate
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(LoadVertex), (GLvoid *)offsetof(LoadVertex, texCoords));

    glBindVertexArray(0);

    numIndices = loadMesh.indices.size();
    material.initialize(loadMesh.material, list);
}

void Mesh::free() {
    if (vao) glDeleteVertexArrays(1, &vao);
	if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}

void Mesh::render(const Shader& shader, bool withMaterial) const {
	if (vao == 0) {
		return;
	}

    if (withMaterial) {
        material.render(shader);
    }

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
