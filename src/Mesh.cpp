
#include "Mesh.h"
#include <iostream>

using namespace std;

Mesh::Mesh() {
    mVao = 0;
    mVbo = 0;
    mEbo = 0;
    mHasGenerated = false;
}

void Mesh::free_resources() {
    if (mHasGenerated) {
        glDeleteVertexArrays(1, &this->mVao);
        glDeleteBuffers(1, &this->mVbo);
        glDeleteBuffers(1, &this->mEbo);
    }

    mHasGenerated = false;
}

void Mesh::generate() {
    // Generate our buffers
    glGenVertexArrays(1, &this->mVao);
    glGenBuffers(1, &this->mVbo);
    glGenBuffers(1, &this->mEbo);

    // Bind data to VAO
    glBindVertexArray(this->mVao);

    // Put the vertex data into OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, this->mVbo);
    glBufferData(GL_ARRAY_BUFFER, this->mVertices.size() * sizeof(Vertex),
                 &this->mVertices[0], GL_STATIC_DRAW);

    // Put the index data into OpenGL
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 this->mIndices.size() * sizeof(GLuint), &this->mIndices[0],
                 GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)0);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, normal));

    // Texture Coordinate
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid *)offsetof(Vertex, texCoords));

    // Unbind the VAO
    glBindVertexArray(0);

    mHasGenerated = true;
}

void Mesh::render(const Shader &shader, bool withMaterial) const {
    if (withMaterial) {
        render_material(shader, mMaterial);
    }

    glBindVertexArray(this->mVao);
    glDrawElements(GL_TRIANGLES, this->mIndices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::add_vertex(Vertex vertex) { this->mVertices.push_back(vertex); }

void Mesh::add_index(GLint index) { this->mIndices.push_back(index); }

void Mesh::set_material(Material material) { this->mMaterial = material; }
