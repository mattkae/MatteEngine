#ifndef SKYBOX_H
#define SKYBOX_H

#include "gl_includes.h"
#include "Shader.h"
#include "Camera.h"

struct Skybox {
  GLuint texture;
  GLuint vao, vbo, ebo;
};

bool initialize_skybox(Skybox& box, const char** paths);
void render_skybox(const Skybox& box, Shader& shader, const Camera& camera);

#endif
