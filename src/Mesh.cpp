
#include "Mesh.h"
#include "LoadModel.h"

using namespace std;

struct Vertex {
    Vector3f position;
    Vector3f normal;
    Vector2f texCoords;
    GLfloat boneWeights[4];
    GLuint boneIndices[4];

    void initialize(const LoadVertex& vertex) {
        position = vertex.position;
        normal = vertex.normal;
        texCoords = vertex.texCoords;

        size_t max = fmin(vertex.boneInfoList.size(), 4);
        for (size_t index = 0; index < max; index++) {
            boneWeights[index] = vertex.boneInfoList[index].weight;
            boneIndices[index] = vertex.boneInfoList[index].boneIndex;
        }
    }
};

void Mesh::initialize(LoadMesh& loadMesh, List* list) {
    Vertex* vertices = new Vertex[loadMesh.vertices.size()];
    for (size_t index = 0; index < loadMesh.vertices.size(); index++) {
        vertices[index].initialize(loadMesh.vertices[index]);
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

    // Texture Coordinate
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, texCoords));

    // Bone weights
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, boneWeights));

    // Bone indices
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(4, 4, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, boneIndices));

    glBindVertexArray(0);

    numIndices = loadMesh.indices.size();
    material.initialize(loadMesh.material, list);

    delete []vertices;
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
