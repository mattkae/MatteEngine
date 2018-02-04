#include "ImageDrawer.h"
#include "Shader.h"
#include "Texture.h"
#include <iostream>

using namespace std;

float vertices[] = {
    // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {
  0, 1, 3, // first triangle
  1, 2, 3  // second triangle
};

ImageDrawer::ImageDrawer() {
  mShader = Shader("src/shaders/cubemap.vert", "src/shaders/cubemap.frag");
  gen_buffers();
  mTexture = nullptr;
  mShader.Use();
  glUniform1i(glGetUniformLocation(mShader.GetProgram(), "ourTexture"), 0);
}

ImageDrawer::ImageDrawer(Texture* texture) {
  mShader = Shader("assets/tex.vert", "assets/tex.frag");  
  mTexture = texture;
  gen_buffers();
  mShader.Use();
  glUniform1i(glGetUniformLocation(mShader.GetProgram(), "ourTexture"), 0);
}

void ImageDrawer::gen_buffers() {
  glGenVertexArrays(1, &mVao);
  glGenBuffers(1, &mVbo);
  glGenBuffers(1, &mEbo);

  glBindVertexArray(mVao);

  glBindBuffer(GL_ARRAY_BUFFER, mVbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);
}

void ImageDrawer::render() {
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mTexture->id);

  mShader.Use();
  glBindVertexArray(mVao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glEnable(GL_DEPTH_TEST);
}

void ImageDrawer::render(GLuint id) {
  glDisable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, id);

  mShader.Use();
  glBindVertexArray(mVao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glEnable(GL_DEPTH_TEST);
}
