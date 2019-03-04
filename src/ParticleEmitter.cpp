#include "ParticleEmitter.h"
#include "Camera.h"
#include "GlmUtility.h"
#include "Shader.h"
#include <array>
#include <cmath>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <vector>

using namespace std;

#define PI 3.14159265

void to_json(nlohmann::json& j, const ParticleFunctionConfig<glm::vec3>& config)
{
}

void from_json(const nlohmann::json& j, ParticleFunctionConfig<glm::vec3>& functionConfig)
{
    glm::from_json(j, "initialVelocity", functionConfig.initialVelocity);
    glm::from_json(j, "linear", functionConfig.linear);
    glm::from_json(j, "quadratic", functionConfig.quadratic);
}

void to_json(nlohmann::json& j, const ParticleFunctionConfig<glm::vec4>& config)
{
}

void from_json(const nlohmann::json& j, ParticleFunctionConfig<glm::vec4>& functionConfig)
{
    glm::from_json(j, "initialVelocity", functionConfig.initialVelocity);
    glm::from_json(j, "linear", functionConfig.linear);
    glm::from_json(j, "quadratic", functionConfig.quadratic);
}

void to_json(nlohmann::json& j, const ParticleEmitter& emitter)
{
}

void from_json(const nlohmann::json& j, ParticleEmitter& emitter)
{
    glm::from_json(j, "model", emitter.model);
    glm::from_json(j, "volumeDimension", emitter.volumeDimension);
    glm::from_json(j, "particleDimension", emitter.particleDimension);
    glm::from_json(j, "origin", emitter.origin);
    j.at("rotationArray").get_to(emitter.rotationArray);
    j.at("numVertices").get_to(emitter.numVertices);
    j.at("particleLifeRangeSeconds").get_to(emitter.particleLifeRangeSeconds);
    j.at("spawnFrequencyRangeSeconds").get_to(emitter.spawnFrequencyRangeSeconds);
    j.at("colorFunction").get_to(emitter.colorFunction);
    j.at("movementFunction").get_to<ParticleFunctionConfig<glm::vec3>>(emitter.movementFunction);

    emitter.generate();
}

template <typename T>
T ParticleFunctionConfig<T>::calculate(float fractionComplete) const
{
    return initialVelocity + (linear * fractionComplete) + (quadratic * fractionComplete * fractionComplete);
}

void ParticleEmitter::onGetParticle(ObjectPoolItem<Particle>& item)
{
    item.value.velocity = movementFunction.initialVelocity;
    item.value.position = glm::get_random_within(origin, volumeDimension);
    item.value.color = colorFunction.initialVelocity;
    item.value.timeAliveSeconds = 0;
    item.value.deathTimeSeconds = lifeFrequencyDistribution(generator);
    item.value.rotation = glm::toMat4(glm::get_random_quaternion(rotationArray));
}

void ParticleEmitter::onRetParticle(ObjectPoolItem<Particle>& particle)
{
}

ParticleEmitter::ParticleEmitter()
{
}

void ParticleEmitter::generate()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    // Times two for vertex position + color
    int verticesCount = 2 * 3 * numVertices;
    GLfloat* vertices = new GLfloat[verticesCount];
    float theta = (360.f / numVertices);

    // Vertices
    int index = 0;
    for (int vidx = 0; vidx < numVertices; vidx++) {
        float thetaRadians = ((theta * vidx) * PI) / 180.f;
        vertices[index] = cos(thetaRadians) * particleDimension.x;
        vertices[index + 1] = sin(thetaRadians) * particleDimension.y;
        vertices[index + 2] = 1.0f;
        index += 3;
    }

    // Indices
    GLuint* indices = new GLuint[numVertices];
    for (int iidx = 0; iidx < numVertices; iidx++) {
        indices[iidx] = iidx;
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numVertices * sizeof(GLuint), indices, GL_STATIC_DRAW);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindVertexArray(0);

    delete[] vertices;
    delete[] indices;

    lifeFrequencyDistribution = std::uniform_real_distribution<double>(particleLifeRangeSeconds[0], particleLifeRangeSeconds[1]);
    spawnFrequencyDistribution = std::uniform_real_distribution<double>(spawnFrequencyRangeSeconds[0], spawnFrequencyRangeSeconds[1]);

    particlePool.onGet = std::bind(&ParticleEmitter::onGetParticle, *this, std::placeholders::_1);
    particlePool.onRet = std::bind(&ParticleEmitter::onRetParticle, *this, std::placeholders::_1);

    isGenerated = true;
}

void ParticleEmitter::update(double deltaTime)
{
    if (!isGenerated) {
        return;
    }

    // Try spawn new particle
    timeUntilSpawnSeconds -= deltaTime;
    if (timeUntilSpawnSeconds <= 0) {
        particlePool.get();
        timeUntilSpawnSeconds = spawnFrequencyDistribution(generator);
    }

    for (auto& item : particlePool.getValues()) {
        if (!item.isTaken) {
            continue;
        }

        Particle& particle = item.value;
        particle.timeAliveSeconds += deltaTime;
        if (particle.timeAliveSeconds > particle.deathTimeSeconds) {
            particlePool.ret(item);
        } else {
            particle.position = particle.position + particle.velocity * ((float)deltaTime);
            particle.velocity = movementFunction.calculate(particle.timeAliveSeconds / particle.deathTimeSeconds);
            particle.color = colorFunction.calculate(particle.timeAliveSeconds / particle.deathTimeSeconds);
        }
    }
}

void ParticleEmitter::render(const Shader& shader, const Camera& camera) const
{
    if (!isGenerated) {
        return;
    }

    camera.render(shader);

    glBindVertexArray(vao);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (auto const& particlePair : particlePool.getConstValues()) {
        if (!particlePair.isTaken) {
            continue;
        }

        const Particle& particle = particlePair.value;

        shader.set_uniform_matrix_4fv("uModel", 1, GL_FALSE, glm::value_ptr(model));
        shader.set_uniform_1f("uTransparency", 1 - particle.timeAliveSeconds / particle.deathTimeSeconds);
        shader.set_uniform_3f("uPosition", particle.position.x, particle.position.y, particle.position.z);
        shader.set_uniform_4f("uColor", particle.color[0], particle.color[1], particle.position[2], particle.color[3]);
        glDrawElements(drawType, numVertices, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void ParticleEmitter::free()
{
}
