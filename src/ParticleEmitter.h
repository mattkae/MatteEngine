#ifndef PARTICLE_H
#define PARTICLE_H
#include "GlmUtility.h"
#include "Shader.h"
#include "Camera.h"
#include <GL/glew.h>
#include <functional>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

struct ParticleRenderVariables {
    glm::vec3 position = glm::vec3(0);
    glm::vec4 color = glm::vec4(1.0);
};

struct ParticleUpdateVariables {
    glm::vec3 velocity = glm::vec3(0, 1, 0);

    float timeAliveSeconds = 0;
    float deathTimeSeconds = 0;
};

template <typename T>
struct ParticleFunctionConfig {
    T initialVelocity;
    T linear;
    T quadratic;
    T calculate(float fractionComplete) const;
};

struct ParticleEmitter {
public:
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 spawnSpread = glm::vec3(1.0f);
    glm::vec3 particleDimension = glm::vec3(1.0f);
    int numVertices = 3;
	int maxParticles = 500;
    glm::vec2 particleLifeSpreadSeconds = glm::vec2(1.0f);
    glm::vec2 spawnFrequencySpreadSeconds = glm::vec2(1.0f);
    ParticleFunctionConfig<glm::vec4> colorFunction;
    ParticleFunctionConfig<glm::vec3> movementFunction;

	ParticleRenderVariables* particleRenderVariables = nullptr;
	ParticleUpdateVariables* particleUpdateVariables = nullptr;
	size_t nextParticleIndex = 0;
    Shader mParticleShader;
    float timeUntilSpawnSeconds = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
	GLuint instanceVbo = 0;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> lifeFrequencyDistribution;
    std::uniform_real_distribution<double> spawnFrequencyDistribution;
};

void initializeParticleEmitter(ParticleEmitter& emitter);
void updateParticleEmitter(ParticleEmitter& emitter, float deltaTimeSec);
void renderParticleEmitter(const ParticleEmitter& emitter, const BetterCamera& camera);
void freeParticleEmitter(ParticleEmitter& emitter);

void to_json(nlohmann::json& j, const ParticleFunctionConfig<glm::vec3>& config);
void from_json(const nlohmann::json& j, ParticleFunctionConfig<glm::vec3>& functionConfig);
void to_json(nlohmann::json& j, const ParticleFunctionConfig<glm::vec4>& config);
void from_json(const nlohmann::json& j, ParticleFunctionConfig<glm::vec4>& functionConfig);
void to_json(nlohmann::json& j, const ParticleEmitter& emitter);
void from_json(const nlohmann::json& j, ParticleEmitter& emitter);

#endif
