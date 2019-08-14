#ifndef PARTICLE_H
#define PARTICLE_H
#include "GlmUtility.h"
#include "ObjectPool.h"
#include "Shader.h"
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
    glm::vec4 color = glm::vec4(1.0);

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
    ParticleEmitter();
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 volumeDimension = glm::vec3(1.0f);
    glm::vec3 particleDimension = glm::vec3(1.0f);
    glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
    std::array<bool, 3> rotationArray { false, true, false };

    int numVertices = 3;
    std::array<double, 2> particleLifeRangeSeconds { 0.5, 1 };
    std::array<double, 2> spawnFrequencyRangeSeconds { 1, 2 };
    GLenum drawType = GL_TRIANGLE_FAN;

    ParticleFunctionConfig<glm::vec4> colorFunction;
    ParticleFunctionConfig<glm::vec3> movementFunction;

    void generate();
    void update(float deltaTimeSec);
    void render(const Camera& camera) const;
    void free();
    void onGetParticle(ObjectPoolItem<Particle>& particle);
    void onRetParticle(ObjectPoolItem<Particle>& particle);

private:
    Shader mParticleShader;
    bool isGenerated = false;
    double timeUntilSpawnSeconds = 0;
    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;
    ObjectPool<Particle> particlePool;
    std::default_random_engine generator;
    std::uniform_real_distribution<double> lifeFrequencyDistribution;
    std::uniform_real_distribution<double> spawnFrequencyDistribution;
};

void to_json(nlohmann::json& j, const ParticleFunctionConfig<glm::vec3>& config);
void from_json(const nlohmann::json& j, ParticleFunctionConfig<glm::vec3>& functionConfig);
void to_json(nlohmann::json& j, const ParticleFunctionConfig<glm::vec4>& config);
void from_json(const nlohmann::json& j, ParticleFunctionConfig<glm::vec4>& functionConfig);
void to_json(nlohmann::json& j, const ParticleEmitter& emitter);
void from_json(const nlohmann::json& j, ParticleEmitter& emitter);

#endif
