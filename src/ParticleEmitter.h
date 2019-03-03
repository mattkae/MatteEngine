#ifndef PARTICLE_H
#define PARTICLE_H
#include "ObjectPool.h"
#include "GlmUtility.h"
#include <GL/glew.h>
#include <functional>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>

class Shader;
class Camera;

struct Particle {
    glm::vec3 position = glm::vec3(0);
    glm::vec3 velocity = glm::vec3(0, 1, 0);
    glm::mat4 rotation = glm::mat4(0);

    double timeAliveSeconds = 0;
	double deathTimeSeconds = 0;
};

struct ParticleFunctionConfig {
    glm::vec3 initialVelocity = glm::vec3(0);
    glm::vec3 linear = glm::vec3(0);
    glm::vec3 quadratic = glm::vec3(0);
	glm::vec3 calculate(float t) const;
};

struct ParticleColorConfig {
    bool isColorStatic = false;
    glm::vec3 startColor = glm::vec3(0);
    glm::vec3 endColor = glm::vec3(0);
    ParticleFunctionConfig function;
};

struct ParticleEmitter {
  public:
	ParticleEmitter();
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 volumeDimension = glm::vec3(1.0f);
    glm::vec3 particleDimension = glm::vec3(1.0f);
    glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	std::array<bool, 3> rotationArray{ false, true, false };
	
    int numVertices = 3;
	std::array<double, 2> particleLifeRangeSeconds{0.5, 1};
	std::array<double, 2> spawnFrequencyRangeSeconds{1, 2};
    GLenum drawType = GL_TRIANGLE_FAN;

    ParticleColorConfig colorConfig;
	ParticleFunctionConfig movementFunction;

    void generate();
    void update(double deltaTimeSec);
    void render(const Shader &shader, const Camera &camera) const;
    void free();
	void onGetParticle(ObjectPoolItem<Particle>& particle);
	void onRetParticle(ObjectPoolItem<Particle>& particle);

  private:
	bool isGenerated = false;
	double timeUntilSpawnSeconds = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
	ObjectPool<Particle> particlePool;
	std::default_random_engine generator;
	std::uniform_int<uint32_t> lifeFrequencyDistribution;
	std::uniform_int<uint32_t> spawnFrequencyDistribution;
};


void to_json(nlohmann::json &j, const ParticleFunctionConfig &config);
void from_json(const nlohmann::json &j, ParticleFunctionConfig &functionConfig);
void to_json(nlohmann::json &j, const ParticleColorConfig &config);
void from_json(const nlohmann::json &j, ParticleColorConfig &colorConfig);
void to_json(nlohmann::json &j, const ParticleEmitter &emitter);
void from_json(const nlohmann::json &j, ParticleEmitter &emitter);

#endif
