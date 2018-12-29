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
};

void to_json(json &j, const Skybox &skybox);
void from_json(const json &j, Skybox &skybox);

bool initialize_skybox(Skybox &box, std::string* paths);
void render_skybox(const Skybox &box, const Shader &shader, const Camera &camera);
void free_resources(Skybox &box);

#endif
