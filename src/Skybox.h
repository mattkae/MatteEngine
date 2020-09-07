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

    void initialize(const char paths[6][128]);
    void render(const Camera& camera) const;
    void free();
};


#endif
