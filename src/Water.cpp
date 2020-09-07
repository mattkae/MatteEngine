#include "Water.h"
#include "ShaderUniformMapping.h"
#include "Vector3f.h"
#include "GridHelper.h"
#include "Camera.h"
#include "MathHelper.h"
#include "Light.h"

void Water::initialize(Camera* inCamera, WaterParameters* waterIn) {
	camera = inCamera;
	GridParameters gridParams;
	gridParams.width = waterIn->width;
	gridParams.height = waterIn->height;
	gridParams.verticesPerUnit = waterIn->verticesPerUnit;
	GridResult gridResult = GridHelper::allocateGridIndices(&gridParams);
	vertices.growDynamically = false;
	vertices.allocate(gridResult.numVertices);
	numIndices = gridResult.numIndices;
	material.diffuse = Vector3f { 0, 0, 0.3 };
	material.transparency = 0.9f;
	material.specularComponent = 20.f;
	material.specular = Vector3f { 0, 0, 0.1 };
	material.ambient = Vector3f { 0, 0, 0 };
	material.emissive = Vector3f { 0, 0, 0 };
	material.specularProperty = Vector3f { 1, 0, 0 };
	modelMatrix = translateMatrix(modelMatrix, getVec3(-waterIn->width / 2.f, 0, -waterIn->height / 2.f));

	for (int row = 0; row <= gridResult.verticesHeight; row++) {
		for (int col = 0; col <= gridResult.verticesWidth; col++) {
			WaterVertex vertex;
			vertex.position = Vector3f { 
				(float)col / waterIn->verticesPerUnit, 
				0.f, 
				(float)row / waterIn->verticesPerUnit 
			};
			
			vertex.periodOffset = (((row * gridResult.verticesHeight) + col) * gridParams.verticesPerUnit) * periodOffsetGradient;
			vertices.add(&vertex);
		}
	}

	glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, gridResult.numVertices * sizeof(WaterVertex), vertices.data, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, gridResult.numIndices * sizeof(GLuint), &gridResult.indices[0], GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), (GLvoid *)0);

	// Period offset
	glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), (GLvoid *)offsetof(WaterVertex, periodOffset));

	glBindVertexArray(0);

	delete[] gridResult.indices;
}

void Water::update(float dt) {
	timePassedMs += dt;
}

void Water::render(Light* lights, unsigned int numLightsUsed) const {
	if (!camera) {
		return;
	}

	useShader(ShaderUniformMapping::GlobalWaterShaderMapping.shader);
	renderCamera(*camera, ShaderUniformMapping::GlobalWaterShaderMapping.cameraMapping);
	setShaderMat4(ShaderUniformMapping::GlobalWaterShaderMapping.UNIFORM_MODEL, modelMatrix);
	setShaderFloat(ShaderUniformMapping::GlobalWaterShaderMapping.UNIFORM_TIME_PASSED_MS, timePassedMs);
	setShaderFloat(ShaderUniformMapping::GlobalWaterShaderMapping.UNIFORM_PERIOD, period);
	setShaderFloat(ShaderUniformMapping::GlobalWaterShaderMapping.UNIFORM_AMPLITUDE, amplitude);

	setShaderVec3(ShaderUniformMapping::GlobalWaterShaderMapping.lightUniformMapping.LIGHT_AMBIENT, getVec3(0.3f));
	setShaderInt(ShaderUniformMapping::GlobalWaterShaderMapping.lightUniformMapping.LIGHT_NUM_LIGHTS, numLightsUsed);

    for (size_t lidx = 0; lidx < numLightsUsed; lidx++) {
		lights[lidx].render(lidx, &ShaderUniformMapping::GlobalWaterShaderMapping.lightUniformMapping);
    }

	material.render(ShaderUniformMapping::GlobalWaterShaderMapping.materialUniformMapping);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Water::free() {
	if (vao > 0) {
		glDeleteVertexArrays(1, &vao);
		vao = 0;
	}
	if (vbo > 0) {
		glDeleteBuffers(1, &vbo);
		vbo = 0;
	}
	if (ebo > 0) {
		glDeleteBuffers(1, &ebo);
		ebo = 0;
	}

	vertices.deallocate();
}