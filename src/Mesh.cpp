
#include "Mesh.h"
#include "LoadModel.h"
#include "Vertex.h"
#include "TextureInfo.h"
#include "List.h"

void Mesh::enableVertexData(Vertex* vertices, unsigned int numVertices, GLint* indices, unsigned int numIndices) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));

    // Tangent
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, tangent));

    // Bitangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, bitangent));

    // Texture Coordinate
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texCoords));

    // Bone weights
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, boneWeights));

    // Bone indices
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(6, 4, GL_INT, sizeof(Vertex), (GLvoid *)offsetof(Vertex, boneIndices));

    glBindVertexArray(0);
}

void Mesh::initialize(LoadMesh& loadMesh, List<GeneratedTexture>* list) {
    Vertex* vertices = new Vertex[loadMesh.vertices.numElements];
    for (size_t index = 0; index < loadMesh.vertices.numElements; index++) {
        Vertex* v = &vertices[index];
        v->initialize(loadMesh.vertices[index]);
    }

    enableVertexData(vertices, loadMesh.vertices.numElements, &loadMesh.indices[0], loadMesh.indices.numElements);

    numIndices = loadMesh.indices.numElements;
    material.initialize(loadMesh.material, list);

    delete []vertices;
}

void Mesh::initialize(Vertex* vertices, 
    unsigned int numVertices, 
    GLint* indices, 
    unsigned int inNumIndices,
    LoadMaterial& loadMat) {

    enableVertexData(vertices, numVertices, indices,inNumIndices);

    numIndices = inNumIndices;
    material.initialize(loadMat, nullptr);
}

void Mesh::initializeFromVertices(List<Vertex>* vertices, List<GLint>* indices) {
    enableVertexData(vertices->data, vertices->numElements, indices->data, indices->numElements);
	numIndices = indices->numElements;
}

void Mesh::free() {
    if (vao) glDeleteVertexArrays(1, &vao);
	if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}

void Mesh::render(const MaterialUniformMapping& mapping, bool withMaterial, GLint drawType) const {
	if (vao == 0) {
		return;
	}

    if (withMaterial) {
        material.render(mapping);
    }

    glBindVertexArray(vao);
    glDrawElements(drawType, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
