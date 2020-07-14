
#include "Mesh.h"
#include "LoadModel.h"
#include "Vertex.h"
#include "TextureInfo.h"
#include "List.h"

void Mesh::initialize(LoadMesh& loadMesh, List<GeneratedTexture>* list) {
    Vertex* vertices = new Vertex[loadMesh.vertices.size()];
    for (size_t index = 0; index < loadMesh.vertices.size(); index++) {
        Vertex* v = &vertices[index];
        v->initialize(loadMesh.vertices[index]);
    }

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, loadMesh.vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, loadMesh.indices.size() * sizeof(GLuint), &loadMesh.indices[0], GL_STATIC_DRAW);

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

    // Texture weights
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, textureWeights));

    // Bone weights
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, boneWeights));

    // Bone indices
    glEnableVertexAttribArray(7);
    glVertexAttribIPointer(7, 4, GL_INT, sizeof(Vertex), (GLvoid *)offsetof(Vertex, boneIndices));

    glBindVertexArray(0);

    numIndices = loadMesh.indices.size();
    material.initialize(loadMesh.material, list);

    delete []vertices;
}

void Mesh::initialize(Vertex* vertices, 
    unsigned int numVertices, 
    GLint* indices, 
    unsigned int inNumIndices,
    LoadMaterial& loadMat) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, inNumIndices * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, normal));

    // Texture Coordinate
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texCoords));

    // Texture weights
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, textureWeights));

    // Bone weights
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, boneWeights));

    // Bone indices
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (GLvoid *)offsetof(Vertex, boneIndices));

    glBindVertexArray(0);

    numIndices = inNumIndices;
    material.initialize(loadMat, nullptr);
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
