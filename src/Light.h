#ifndef LIGHT_H
#define LIGHT_H

#include "Model.h"
#include "Shader.h"
#include "GlobalApplicationState.h"
#include "Vector3f.h"
#include "Matrix4x4f.h"
#include <GL/glew.h>
#include <vector>

struct BetterScene;

enum LightType { Directional = 0, PointLight, Spot, Inactive };

struct Light {
	GLint index = 0;
	LightType type = LightType::Directional;
    Vector3f color = Vector3f(1.0);
    Vector3f direction = Vector3f(0.0, 0.0, -1.0);
    Vector3f up = Vector3f(0.0, 1.0, 0.0);
    Vector3f position = Vector3f(0.0);

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

	Matrix4x4f projection;
	Matrix4x4f view;
	
	GLint colorUniform;
	GLint directionUniform;
	GLint positionUniform;
	GLint constantUniform;
	GLint linearUniform;
	GLint quadraticUniform;
	GLint cosineCutoffUniform;
	GLint dropOffUniform;
	GLint usesShadowsUniform;
	GLint dirShadowUniform;
	GLint shadowMatrixUniform;
};

bool initLight(Light& light, const Shader& shader, int lightIndex);
void renderLightShadows(const Light& light, const Shader shader, const BetterScene& scene);
void renderLight(const Light& light, const Shader shader, const int index);
void freeLight(Light& light);
#endif
