#ifndef LIGHT_H
#define LIGHT_H

#include "Model.h"
#include "Shader.h"
#include "GlobalApplicationState.h"
#include "Vector3f.h"
#include "Matrix4x4f.h"
#include <GL/glew.h>
#include <vector>

struct Scene;

enum LightType { Directional = 0, PointLight, Spot, Inactive };

struct Light {
	GLint index = 0;
	LightType type = LightType::Directional;
    Vector3f color = getVec3(1.0);
    Vector3f direction = getVec3(0.0, 0.0, -1.0);
    Vector3f up = getVec3(0.0, 1.0, 0.0);
    Vector3f position = getVec3(0.0);

    GLfloat constant = 1.0f;
    GLfloat linear = 0.0f;
    GLfloat quadratic = 0.f;

    GLfloat cosineCutOff = 1.0f;
    GLfloat dropOff = 1.0f;

    bool usesShadows = false;

	GLint shadowWidth = 2 * GlobalAppState.width;
    GLint shadowHeight = 2 * GlobalAppState.height;

	GLuint shadowTexture = 0;
	GLuint depthFbo = 0;
	bool isOn = true;

	Matrix4x4f projection;
	Matrix4x4f view;

    void initialize(int lightIndex);
    void renderShadows(const Scene& scene);
    void render(const int index) const;
    void free();
};

#endif
