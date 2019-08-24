
#include "Mesh.h"
#include <iostream>

using namespace std;

void freeMesh(Mesh& mesh) {
    if (mesh.vao) glDeleteVertexArrays(1, &mesh.vao);
	if (mesh.vbo) glDeleteBuffers(1, &mesh.vbo);
    if (mesh.ebo) glDeleteBuffers(1, &mesh.ebo);
}

void initializeMesh(Mesh& mesh) {
    // Generate our buffers
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    // Bind data to VAO
    glBindVertexArray(mesh.vao);

    // Put the vertex data into OpenGL
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex),
                 &mesh.vertices[0], GL_STATIC_DRAW);

    // Put the index data into OpenGL
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mesh.indicies.size() * sizeof(GLuint), &mesh.indicies[0],
                 GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
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
}

void renderMesh(const Mesh& mesh, const Shader& shader, bool withMaterial) {
	if (mesh.vao == 0) {
		return;
	}

    if (withMaterial) {
        render_material(shader, mesh.material);
    }

    glBindVertexArray(mesh.vao);
    glDrawElements(GL_TRIANGLES, mesh.indicies.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
