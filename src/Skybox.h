#ifndef SKYBOX_H
#define SKYBOX_H

#include "Camera.h"
#include "Shader.h"
#include <GL/glew.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Skybox {
    GLuint mTexture;
    GLuint vao = 0, vbo = 0, ebo = 0;
};

void initSkybox(Skybox& skybox, const char paths[6][128]);
void renderSkybox(const Skybox& skybox, const BetterCamera &camera);
void freeSkybox(Skybox& skybox);

#endif
