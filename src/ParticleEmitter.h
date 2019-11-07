#ifndef PARTICLE_H
#define PARTICLE_H
#include "GlmUtility.h"
#include "Shader.h"
#include "Camera.h"
#include "Randomizable.h"
#include "Functional.h"
#include <GL/glew.h>
#include <functional>
#include <glm/glm.hpp>

struct ParticleRenderVariables {
    glm::vec3 position = glm::vec3(0);
    glm::vec4 color = glm::vec4(1.0);
};

struct ParticleUpdateVariables {
    glm::vec3 velocity = glm::vec3(0, 1, 0);
    float timeAliveSeconds = 0;
    float deathTimeSeconds = 0;
};

struct ParticleEmitter {
public:
	// Group variables
    glm::mat4 model = glm::mat4(1.0f);
	RandomizableFloat spawnFrequencySeconds;
	size_t maxParticles = 500;
	FunctionVec4 colorFunction;
    FunctionVec3 movementFunction;
    int numVertices = 3;
	glm::vec2 particleDimension;

	// Per particle variables
	RandomizableVec3 particlePosition;
	RandomizableFloat particleTimeToLiveSeconds;

	// Internal
	ParticleRenderVariables* particleRenderVariables = nullptr;
	ParticleUpdateVariables* particleUpdateVariables = nullptr;
	size_t nextParticleIndex = 0;
    Shader mParticleShader;
    float timeUntilSpawnSeconds = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
	GLuint instanceVbo = 0;
};

void initializeParticleEmitter(ParticleEmitter& emitter, size_t initialParticleCount);
void updateParticleEmitter(ParticleEmitter& emitter, float deltaTimeSec);
void renderParticleEmitter(const ParticleEmitter& emitter, const BetterCamera& camera);
void freeParticleEmitter(ParticleEmitter& emitter);

#endif
