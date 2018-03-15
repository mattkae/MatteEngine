#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

using namespace std;

Terrain generate_terrain(int dimension, int granularity) {
  Terrain terrain;
  glGenVertexArrays(1, &terrain.vao);
  glGenBuffers(1, &terrain.vbo);
  glGenBuffers(1, &terrain.ebo);
  glBindVertexArray(terrain.vao);

  float squareSize = ((float)dimension / (float)granularity) / 2.f;
  float vertexArrySize = 3 * granularity * granularity;
  float indexArrySize = 6 * granularity * granularity;

  // Generate vertices
  int index = 0;
  GLfloat* vertices = new GLfloat[vertexArrySize];
  for (int r = 0; r < granularity; r++) {
    for (int c = 0; c < granularity; c++) {
      vertices[index] = c * squareSize;
      vertices[index + 1] = 0;
      vertices[index + 2] = r * squareSize;

      index += 3;
    }
  }

  // Generate indices
  index = 0;
  GLuint* indices = new GLuint[indexArrySize];
  for (GLuint r = 0; r < granularity - 1; r++) {
    for (GLuint c = 0; c < granularity - 1; c++) {
      if (r % 2 == 0) {
	GLuint idx = r * granularity + c;
	indices[index] = idx;
	indices[index + 1] = idx + 1;
	indices[index + 2] = idx + granularity;
	indices[index + 3] = idx + granularity;
	indices[index + 4] = idx + granularity + 1;
	indices[index + 5] = idx + 1;
      } else {
	GLuint idx = r * granularity + (granularity - c - 1);
	indices[index] = idx;
	indices[index + 1] = idx - 1;
	indices[index + 2] = idx + granularity;
	indices[index + 3] = idx + granularity;
	indices[index + 4] = idx + granularity - 1;
	indices[index + 5] = idx - 1;
      }
	
      index += 6;
    }
  }

   // Put the vertex data into OpenGL
  glBindBuffer(GL_ARRAY_BUFFER, terrain.vbo);
  glBufferData(GL_ARRAY_BUFFER, vertexArrySize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

  // Put the index data into OpenGL
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArrySize * sizeof(GLuint), indices, GL_STATIC_DRAW);

  // Position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*) 0);
  glBindVertexArray(0);

  terrain.numIndices = indexArrySize;
  delete vertices;
  delete indices;
  return terrain;
}

glm::mat4 get_viewport() {
  glm::mat4 s  = glm::scale(glm::mat4(1.0), glm::vec3(Constants.width / 2.f, Constants.height / 2.f, 1.f / 2.f));
  glm::mat4 t = glm::translate(glm::mat4(1.0), glm::vec3(Constants.width / 2.f, Constants.height / 2.f, 1.f / 2.f)); 
  return t * s;
}

void render_terrain(const Terrain& terrain, const Shader& shader, const Camera& camera) {
  camera.render(shader);
  glDisable(GL_DEPTH);
  glEnable (GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  shader.SetUniformMatrix4fv("uViewportMatrix", 1, GL_FALSE, glm::value_ptr(get_viewport()));
  shader.SetUniform1f("uLineWidth", 1.0);
  shader.SetUniform4f("uLineColor", 1.0, 1.0, 1.0, 1.0);

  // TODO: We probably no longer need a model matrix?
  glm::mat4 model(1.0);
  shader.SetUniformMatrix4fv("uModel", 1, GL_FALSE, glm::value_ptr(model));

  glBindVertexArray(terrain.vao);
  glDrawElements(GL_TRIANGLES, terrain.numIndices, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  
  glEnable(GL_DEPTH);
  glDisable(GL_BLEND);
}
