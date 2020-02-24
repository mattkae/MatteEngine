#include "ParticleEmitter.h"
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

#define PI 3.14159265f

inline void spawnParticles(ParticleEmitter& emitter, size_t numParticlesToSpawn) {
	while (numParticlesToSpawn > 0 && emitter.nextParticleIndex < emitter.maxParticles) {
		emitter.particleRenderVariables[emitter.nextParticleIndex].color = emitter.colorFunction.initial;
		emitter.particleRenderVariables[emitter.nextParticleIndex].position = getRandom(emitter.particlePosition);
		emitter.particleUpdateVariables[emitter.nextParticleIndex].velocity = emitter.movementFunction.initial;;
		emitter.particleUpdateVariables[emitter.nextParticleIndex].timeAliveSeconds = 0;
		emitter.particleUpdateVariables[emitter.nextParticleIndex].deathTimeSeconds = getRandom(emitter.particleTimeToLiveSeconds);
		emitter.nextParticleIndex++;
		numParticlesToSpawn--;
	}
}

void initializeParticleEmitter(ParticleEmitter& emitter, size_t initialParticleCount) {
	emitter.particleUpdateVariables = new ParticleUpdateVariables[emitter.maxParticles];
	emitter.particleRenderVariables = new ParticleRenderVariables[emitter.maxParticles];
    emitter.mParticleShader = loadShader("src/shaders/particle.vert", "src/shaders/particle.frag");

    int verticesCount = 6 * emitter.numVertices;
    GLfloat* vertices = new GLfloat[verticesCount];
    float theta = (360.f / emitter.numVertices);

	int indexPtr = 0;
    for (int vidx = 0; vidx < emitter.numVertices; vidx++) {
        GLfloat thetaRadians = ((theta * vidx) * PI) / 180.f;
        GLfloat nextThetaRadians = ((theta * (vidx + 1)) * PI) / 180.f;

		// Center point
		int index = vidx * 6;
		vertices[index] = 0;
		vertices[index + 1] = 0;

		// First point
        vertices[index + 2] = static_cast<GLfloat>(cos(thetaRadians) * emitter.particleDimension.x);
        vertices[index + 3] = static_cast<GLfloat>(sin(thetaRadians) * emitter.particleDimension.y);

		// Second point
		vertices[index + 4] = static_cast<GLfloat>(cos(nextThetaRadians) * emitter.particleDimension.x);
		vertices[index + 5] = static_cast<GLfloat>(sin(nextThetaRadians) * emitter.particleDimension.y);
    }

	glGenVertexArrays(1, &emitter.vao);
    glGenBuffers(1, &emitter.vbo);
	glGenBuffers(1, &emitter.instanceVbo);

    glBindVertexArray(emitter.vao);

	// Vertex data
    glBindBuffer(GL_ARRAY_BUFFER, emitter.vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Instance Data
	glBindBuffer(GL_ARRAY_BUFFER, emitter.instanceVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ParticleRenderVariables) * emitter.maxParticles, emitter.particleRenderVariables, GL_STATIC_DRAW);
	// World Position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleRenderVariables), (GLvoid*)0);
	// Color
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleRenderVariables), (GLvoid*)offsetof(ParticleRenderVariables, color));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glVertexAttribDivisor(1, 1);
	glVertexAttribDivisor(2, 1);

    glBindVertexArray(0);

	spawnParticles(emitter, initialParticleCount);
    delete[] vertices;
}

void updateParticleEmitter(ParticleEmitter& emitter, float deltaTimeSec) {
    if (emitter.vao == 0) {
        return;
    }

	float diffTime = emitter.timeUntilSpawnSeconds - deltaTimeSec;
	if (diffTime < 0) {
		size_t numParticlesToSpawn = static_cast<size_t>(ceil(diffTime / -emitter.timeUntilSpawnSeconds));
		spawnParticles(emitter, numParticlesToSpawn);
        emitter.timeUntilSpawnSeconds = getRandom(emitter.spawnFrequencySeconds);
	} else {
		emitter.timeUntilSpawnSeconds = diffTime;
	}

    for (size_t activeIndex = 0; activeIndex < emitter.nextParticleIndex; activeIndex++) {
        ParticleUpdateVariables& updateParticle = emitter.particleUpdateVariables[activeIndex];
        if (updateParticle.timeAliveSeconds > updateParticle.deathTimeSeconds) {
			emitter.particleUpdateVariables[activeIndex] = emitter.particleUpdateVariables[emitter.nextParticleIndex - 1];
			emitter.particleRenderVariables[activeIndex] = emitter.particleRenderVariables[emitter.nextParticleIndex - 1];
			emitter.nextParticleIndex--;
			activeIndex--;
        } else {
			ParticleRenderVariables& renderParticle = emitter.particleRenderVariables[activeIndex];
            renderParticle.position = renderParticle.position + updateParticle.velocity * deltaTimeSec;
            renderParticle.color = calculateFunc(emitter.colorFunction, updateParticle.timeAliveSeconds / updateParticle.deathTimeSeconds);
            updateParticle.velocity = calculateFunc(emitter.movementFunction, updateParticle.timeAliveSeconds / updateParticle.deathTimeSeconds);
			updateParticle.timeAliveSeconds += deltaTimeSec;
        }
    }
}

void debugVAOState(std::string baseMessage)
{
    baseMessage.append( " ... querying VAO state:\n" );
    int vab, eabb, eabbs, mva, isOn( 1 ), vaabb;
    glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &vab );
    glGetIntegerv( GL_ELEMENT_ARRAY_BUFFER_BINDING, &eabb );
    glGetBufferParameteriv( GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &eabbs );

    baseMessage.append( "  VAO: " + std::to_string( vab ) + "\n" );
    baseMessage.append( "  IBO: " + std::to_string( eabb ) + ", size=" + std::to_string( eabbs )  + "\n" );

    glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, &mva );
    for ( unsigned i = 0; i < mva; ++i )
    {
        glGetVertexAttribiv( i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isOn );
        if ( isOn )
        {
            glGetVertexAttribiv( i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vaabb );
            baseMessage.append( "  attrib #" + std::to_string( i ) + ": VBO=" + std::to_string( vaabb ) + "\n" );
        }
    }

	printf("%s\n", baseMessage.c_str());
}

void renderParticleEmitter(const ParticleEmitter& emitter, const BetterCamera& camera) {
    if (emitter.vao == 0) {
        return;
    }

	useShader(emitter.mParticleShader);
    renderCamera(camera, emitter.mParticleShader);

	setShaderMat4(emitter.mParticleShader, "uModel", emitter.model);
	setShaderVec3(emitter.mParticleShader, "uCameraUp", camera.up);
	setShaderVec3(emitter.mParticleShader, "uCameraRight", camera.right);

	glBindVertexArray(emitter.vao);

	glBindBuffer(GL_ARRAY_BUFFER, emitter.instanceVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ParticleRenderVariables) * emitter.maxParticles, emitter.particleRenderVariables);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 6 * emitter.numVertices, emitter.nextParticleIndex);
    glBindVertexArray(0);
}

void freeParticleEmitter(ParticleEmitter& emitter) {
	if (emitter.particleRenderVariables != nullptr) {
		delete[] emitter.particleRenderVariables;
	}

	if (emitter.particleUpdateVariables != nullptr) {
		delete[] emitter.particleUpdateVariables;
	}

	if (emitter.vao > 0) {
		glDeleteVertexArrays(1, &emitter.vao);
	}

	if (emitter.vbo > 0) {
		glDeleteFramebuffers(1, &emitter.vbo);
	}

	if (emitter.instanceVbo > 0) {
		glDeleteFramebuffers(1, &emitter.instanceVbo);
	}

	if (emitter.mParticleShader > 0) {
		glDeleteShader(emitter.mParticleShader);
	}
}
