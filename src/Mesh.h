#ifndef MESH_H
#define MESH_H

#include "Material.h"
#include "Shader.h"
#include <GL/glew.h>
#include <vector>

struct LoadMesh;
struct List;
struct Vertex;

struct Mesh {
    Material material;
    GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLint numIndices = 0;

	void initialize(LoadMesh& loadMesh, List* list);
	void free();
	void render(const Shader& shader, bool withMaterial = true) const;
};

#endif
