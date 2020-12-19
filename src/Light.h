#ifndef LIGHT_H
#define LIGHT_H

#include "Model.h"
#include "Shader.h"
#include "Vector3f.h"
#include "Matrix4x4f.h"
#include <GL/glew.h>
#include <vector>

struct Scene;
struct LightUniformMapping;

enum LightType {
    None        = 0,
    Directional = 1, 
    PointLight  = 2, 
    Spot        = 3
};

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

	GLint shadowWidth = 1280;
    GLint shadowHeight = 960;

	GLuint shadowTexture = 0;
	GLuint depthFbo = 0;
	bool isOn = true;

    Matrix4x4f lightSpaceMatrix;
	Matrix4x4f projection;
	Matrix4x4f view;

    void initialize(int lightIndex);
    void renderShadows(const Scene& scene);
    void render(const LightUniformMapping* uniformMapping) const;
    void free();
};

#endif
