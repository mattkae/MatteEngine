#ifndef PARTICLE_H
#define PARTICLE_H
#include "Shader.h"
#include "Camera.h"
#include "Randomizable.h"
#include "Functional.h"
#include <GL/glew.h>
#include <functional>
#include "Vector2f.h"
#include "Vector3f.h"
#include "Vector4f.h"
#include "Matrix4x4f.h"

struct ParticleRenderVariables {
    Vector3f position;
	Vector4f color = Vector4f{ 1.f, 1.f, 1.f, 1.f };
};

struct ParticleUpdateVariables {
	Vector3f velocity = Vector3f{ 0.f, 1.f, 0.f };
    float timeAliveSeconds = 0;
    float deathTimeSeconds = 0;
};

struct ParticleEmitter {
public:
	// Group variables
    Matrix4x4f model;
	RandomizableFloat spawnFrequencySeconds;
	size_t maxParticles = 500;
	FunctionVec4 colorFunction;
    FunctionVec3 movementFunction;
    int numVertices = 3;
	Vector2f particleDimension;

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
