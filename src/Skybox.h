#ifndef SKYBOX_H
#define SKYBOX_H

#include "Camera.h"
#include "Shader.h"
#include <GL/glew.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

struct Skybox {
public:
    bool generate(std::string* paths);
    void render(const BetterCamera &camera) const;
    void free();

private:
    GLuint mTexture;
    GLuint vao, vbo, ebo;
    bool mIsInited = false;
    Shader mSkyboxShader;
};

void to_json(json &j, const Skybox &skybox);
void from_json(const json &j, Skybox &skybox);

#endif
