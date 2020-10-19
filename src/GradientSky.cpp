#include "GradientSky.h"
#include "App.h"
#include "MathHelper.h"
#include "ShaderUniformMapping.h"
#include "Camera.h"
#include <GL/glew.h>

inline DomeVertex getVertex(float radius, int phi, int theta) {
	DomeVertex vertex;
	vertex.position.x = radius * sinf(phi * DTOR) * cosf(theta * DTOR);
	vertex.position.y = radius * cosf(phi * DTOR);
	vertex.position.z = radius * sinf(phi * DTOR) * sinf(theta * DTOR);
	return vertex;
}

void GradientSky::initialize(float radius, float deltaPhi, float deltaTheta) {
	List<DomeVertex> vertices;
	List<GLuint> indices;
	vertices.growDynamically = false;
	indices.growDynamically = false;

	int numVertices = static_cast<int>((360.f / deltaTheta) * (90.f / deltaPhi) * 4.f);
	numIndices = numVertices * 6;
	vertices.allocate(numVertices);
	indices.allocate(numIndices);

	GLuint index = 0;
	for (int phi = 0; phi <= 90 - deltaPhi; phi += (int)deltaPhi) {
		for (int theta = 0; theta <= 360 - deltaTheta; theta += (int)deltaTheta) {
			DomeVertex vertex = getVertex(radius, phi, theta);
			vertices.add(&vertex);
		    GLuint topLeftIdx = index++;

			vertex = getVertex(radius, phi, theta + deltaTheta);
			vertices.add(&vertex);
		    GLuint bottomLeftIdx = index++;

			vertex = getVertex(radius, phi + deltaPhi, theta + deltaTheta);
			vertices.add(&vertex);
			GLuint bottomRightIdx = index++;

			vertex = getVertex(radius, phi + deltaPhi, theta);
			vertices.add(&vertex);
		    GLuint topRightIdx = index++;

		    indices.add(&topLeftIdx);
            indices.add(&bottomLeftIdx);
            indices.add(&bottomRightIdx);
            indices.add(&bottomRightIdx);
            indices.add(&topLeftIdx);
            indices.add(&topRightIdx);
		}
	}

	glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(DomeVertex), vertices.data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(GLuint), indices.data, GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DomeVertex), (GLvoid *)0);

    glBindVertexArray(0);

	vertices.deallocate();
    indices.deallocate();
}

void GradientSky::render(const Camera* camera) {
    if (vao == 0) {
		return;
	}

	useShader(ShaderUniformMapping::GlobalDomeSkyShaderMapping.shader);
	setShaderVec4(ShaderUniformMapping::GlobalDomeSkyShaderMapping.UNIFORM_LOW_COLOR, startColor);
	setShaderVec4(ShaderUniformMapping::GlobalDomeSkyShaderMapping.UNIFORM_HIGH_COLOR, endColor);
    setShaderVec2(ShaderUniformMapping::GlobalDomeSkyShaderMapping.UNIFORM_MIX_POSITIONS, mixStartEnd);
	camera->render(ShaderUniformMapping::GlobalDomeSkyShaderMapping.cameraMapping);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GradientSky::free() {
	if (vao) glDeleteVertexArrays(1, &vao);
	if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
}
