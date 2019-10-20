#ifndef LIGHT_H
#define LIGHT_H

#include "Model.h"
#include "Shader.h"
#include "GlobalApplicationState.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>

using json = nlohmann::json;

class Scene;

enum LightType { Directional = 0, PointLight, Spot, Inactive };

struct Light {
	LightType type;
    glm::vec3 color = glm::vec3(1.0);
    glm::vec3 direction = glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 position = glm::vec3(0.0);

    GLfloat constant = 1.0f;
    GLfloat linear = 0.0f;
    GLfloat quadratic = 0.f;

    GLfloat cosineCutOff = 1.0f;
    GLfloat dropOff = 1.0f;

    bool usesShadows = false;

	GLint shadowWidth = GlobalAppState.width;
    GLint shadowHeight = GlobalAppState.height;

	GLuint shadowTexture = 0;
	GLuint depthFbo = 0;
	bool isOn = true;

	glm::mat4 projection;
	glm::mat4 view;
};

bool initLight(Light& light);
void renderLightShadows(const Light& light, const Shader shader, const Scene& scene);
void renderLight(const Light& light, const Shader shader, const int index);
void freeLight(Light& light);

void to_json(json &j, const Light &light);
void from_json(const json &j, Light &light);
#endif
