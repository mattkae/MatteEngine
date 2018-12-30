#ifndef SKYBOX_H
#define SKYBOX_H

#include "Camera.h"
#include "Shader.h"
#include <GL/glew.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Skybox {
    GLuint texture;
    GLuint vao, vbo, ebo;
    bool isInited = false;

    bool generate(std::string* paths);
    void render(const Shader &shader, const Camera &camera) const;
    void free();
};

void to_json(json &j, const Skybox &skybox);
void from_json(const json &j, Skybox &skybox);

#endif
