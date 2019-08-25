#ifndef MESH_H
#define MESH_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Material.h"
#include "Shader.h"
#include "Vertex.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<GLint> indicies;
    Material material;
    GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
};

void initializeMesh(Mesh& mesh);
void renderMesh(const Mesh& mesh, const Shader& shader, bool withMaterial = true);
void freeMesh(Mesh& mesh);

#endif
