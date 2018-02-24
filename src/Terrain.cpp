#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

const GLuint indices[] = {
  0, 1, 3,
  1, 2, 3
};


Terrain generate_terrain(int dimension, int granularity) {
  Terrain terrain;
    
  glGenVertexArrays(1, &terrain.vao);
  glGenBuffers(1, &terrain.vbo);
  glGenBuffers(1, &terrain.ebo);
  glBindVertexArray(terrain.vao);

  float squareSize = (dimension / granularity) / 2;
  const GLfloat vertices[] = {
    squareSize, 0, squareSize,
    squareSize, 0, -squareSize,
    -squareSize, 0, -squareSize,
    -squareSize, 0, squareSize
  };

   // Put the vertex data into OpenGL
  glBindBuffer(GL_ARRAY_BUFFER, terrain.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Put the index data into OpenGL
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*) 0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

  int numSquares = dimension / granularity;
  terrain.numSquares = numSquares / 2;
  terrain.granularity = granularity;
  return terrain;
}

void render_terrain(const Terrain& terrain, const Shader& shader, const Camera& camera) {
  camera.render(shader);
  
  glBindVertexArray(terrain.vao);
  
  for (int r = -terrain.numSquares; r < terrain.numSquares; r++) {
    for (int c = -terrain.numSquares; c < terrain.numSquares; c++) {
      float x = c * terrain.granularity;
      float z = r * terrain.granularity;

      if (x > Constants.far || z > Constants.far) {
	continue;
      }

      glm::mat4 model(1.0);
      model = glm::translate(model, glm::vec3(x, 0.0, z));
      shader.SetUniformMatrix4fv("uModel", 1, GL_FALSE, glm::value_ptr(model));
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
  }
  
  glBindVertexArray(0);
}
