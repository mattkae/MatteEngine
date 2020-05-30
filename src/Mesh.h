#ifndef MESH_H
#define MESH_H

#include "Material.h"
#include "Shader.h"
#include "GeneratedTexture.h"
#include <GL/glew.h>
#include <vector>

struct LoadMesh;
struct LoadMaterial;
struct Vertex;

struct Mesh {
    Material material;
    GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLint numIndices = 0;

	void initialize(Vertex* vertices,
		unsigned int numVertices, 
		GLint* indices, 
		unsigned int numIndices,
		LoadMaterial& loadMaterial
	);
	void initialize(LoadMesh& loadMesh, List<GeneratedTexture>* list);
	void free();
	void render(const Shader& shader, bool withMaterial = true, GLint drawType = GL_TRIANGLES) const;
};

#endif
