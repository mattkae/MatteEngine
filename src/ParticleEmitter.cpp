#include "ParticleEmitter.h"
#include "GlmUtility.h"
#include <array>
#include <cmath>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <vector>

using namespace std;

#define PI 3.14159265f

void to_json(nlohmann::json& j, const ParticleFunctionConfig<glm::vec3>& config) {
}

void from_json(const nlohmann::json& j, ParticleFunctionConfig<glm::vec3>& functionConfig)
{
    glm::from_json(j, "initialVelocity", functionConfig.initialVelocity);
    glm::from_json(j, "linear", functionConfig.linear);
    glm::from_json(j, "quadratic", functionConfig.quadratic);
}

void to_json(nlohmann::json& j, const ParticleFunctionConfig<glm::vec4>& config) {
}

void from_json(const nlohmann::json& j, ParticleFunctionConfig<glm::vec4>& functionConfig) {
    glm::from_json(j, "initialVelocity", functionConfig.initialVelocity);
    glm::from_json(j, "linear", functionConfig.linear);
    glm::from_json(j, "quadratic", functionConfig.quadratic);
}

void to_json(nlohmann::json& j, const ParticleEmitter& emitter) {
}

void from_json(const nlohmann::json& j, ParticleEmitter& emitter) {
    glm::from_json(j, "model", emitter.model);
    glm::from_json(j, "spawnSpread", emitter.spawnSpread);
    glm::from_json(j, "particleDimension", emitter.particleDimension);
    j.at("numVertices").get_to(emitter.numVertices);
    glm::from_json(j, "particleLifeRangeSeconds", emitter.particleLifeSpreadSeconds);
    glm::from_json(j, "spawnFrequencyRangeSeconds", emitter.spawnFrequencySpreadSeconds);
    j.at("colorFunction").get_to(emitter.colorFunction);
    j.at("movementFunction").get_to(emitter.movementFunction);
    j.at("maxParticles").get_to(emitter.maxParticles);

    initializeParticleEmitter(emitter);
}

template <typename T>
T ParticleFunctionConfig<T>::calculate(float fractionComplete) const {
    return initialVelocity + (linear * fractionComplete) + (quadratic * fractionComplete * fractionComplete);
}

void initializeParticleEmitter(ParticleEmitter& emitter) {
	emitter.particleUpdateVariables = new ParticleUpdateVariables[emitter.maxParticles];
	emitter.particleRenderVariables = new ParticleRenderVariables[emitter.maxParticles];
    emitter.mParticleShader = loadShader("src/shaders/particle.vert", "src/shaders/particle.frag");
	emitter.lifeFrequencyDistribution = std::uniform_real_distribution<double>(emitter.particleLifeSpreadSeconds.r, emitter.particleLifeSpreadSeconds.g);
    emitter.spawnFrequencyDistribution = std::uniform_real_distribution<double>(emitter.spawnFrequencySpreadSeconds.r, emitter.spawnFrequencySpreadSeconds.g);

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
#if true
    glBindBuffer(GL_ARRAY_BUFFER, emitter.vbo);
    glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#else
	float quadVertices[] = {
        // positions     // colors
        -0.05f,  0.05f,
         0.05f, -0.05f,
        -0.05f, -0.05f,

        -0.05f,  0.05f,
         0.05f, -0.05f,
         0.05f,  0.05f
    };
    glBindBuffer(GL_ARRAY_BUFFER, emitter.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
#endif

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

    delete[] vertices;
}

void updateParticleEmitter(ParticleEmitter& emitter, float deltaTimeSec) {
    if (emitter.vao == 0) {
        return;
    }

	float diffTime = emitter.timeUntilSpawnSeconds - deltaTimeSec;
	if (diffTime < 0) {
		int numParticlesToSpawn = static_cast<int>(ceil(diffTime / -emitter.timeUntilSpawnSeconds));
		while (numParticlesToSpawn > 0 && emitter.nextParticleIndex < emitter.maxParticles) {
			emitter.particleRenderVariables[emitter.nextParticleIndex].color = emitter.colorFunction.initialVelocity;
			emitter.particleRenderVariables[emitter.nextParticleIndex].position = glm::get_random_within(glm::vec3(0), emitter.spawnSpread);
			emitter.particleUpdateVariables[emitter.nextParticleIndex].velocity = emitter.movementFunction.initialVelocity;
			emitter.particleUpdateVariables[emitter.nextParticleIndex].timeAliveSeconds = 0;
			emitter.particleUpdateVariables[emitter.nextParticleIndex].deathTimeSeconds = static_cast<float>(emitter.lifeFrequencyDistribution(emitter.generator));
			emitter.nextParticleIndex++;
			numParticlesToSpawn--;
		}

        emitter.timeUntilSpawnSeconds += emitter.spawnFrequencySpreadSeconds.x;
	} else {
		emitter.timeUntilSpawnSeconds = diffTime;
	}

    for (int activeIndex = 0; activeIndex < emitter.nextParticleIndex; activeIndex++) {
        ParticleUpdateVariables& updateParticle = emitter.particleUpdateVariables[activeIndex];
        if (updateParticle.timeAliveSeconds > updateParticle.deathTimeSeconds) {
			emitter.particleUpdateVariables[activeIndex] = emitter.particleUpdateVariables[emitter.nextParticleIndex - 1];
			emitter.particleRenderVariables[activeIndex] = emitter.particleRenderVariables[emitter.nextParticleIndex - 1];
			emitter.nextParticleIndex--;
			activeIndex--;
        } else {
			ParticleRenderVariables& renderParticle = emitter.particleRenderVariables[activeIndex];
            renderParticle.position = renderParticle.position + updateParticle.velocity * deltaTimeSec;
            renderParticle.color = emitter.colorFunction.calculate(updateParticle.timeAliveSeconds / updateParticle.deathTimeSeconds);
            updateParticle.velocity = emitter.movementFunction.calculate(updateParticle.timeAliveSeconds / updateParticle.deathTimeSeconds);
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

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	setShaderMat4(emitter.mParticleShader, "uModel", emitter.model);
	setShaderVec3(emitter.mParticleShader, "uCameraUp", camera.up);
	setShaderVec3(emitter.mParticleShader, "uCameraRight", camera.right);

	printf("%d\n", emitter.nextParticleIndex);

	glBindVertexArray(emitter.vao);

	glBindBuffer(GL_ARRAY_BUFFER, emitter.instanceVbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ParticleRenderVariables) * emitter.maxParticles, emitter.particleRenderVariables);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//debugVAOState("error: ");

#if true
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6 * emitter.numVertices, emitter.nextParticleIndex);
#else
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
#endif
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void freeParticleEmitter(ParticleEmitter& emitter) {
	// FREE!
}
