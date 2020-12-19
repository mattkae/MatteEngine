#ifndef MESH_H
#define MESH_H

#include "Material.h"
#include "Shader.h"
#include "GeneratedTexture.h"
#include <GL/glew.h>

struct LoadMesh;
struct LoadMaterial;
struct Vertex;
struct MaterialUniformMapping;

struct Mesh {
    Material material;
    GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;
	GLint numIndices = 0;

	// @TODO: This initializer is bad, we dont' really need it anymore.
	void initialize(Vertex* vertices,
		unsigned int numVertices, 
		GLint* indices, 
		unsigned int numIndices,
		LoadMaterial& loadMaterial
	);
	void initialize(LoadMesh& loadMesh, List<GeneratedTexture>* list);
	void initializeFromVertices(List<Vertex>* vertices, List<GLint>* indices);
	void enableVertexData(Vertex* vertices, unsigned int numVertices, GLint* indices, unsigned int numIndices);
	void free();
	void render(const MaterialUniformMapping& mapping, bool withMaterial = true, GLint drawType = GL_TRIANGLES) const;
};

#endif
