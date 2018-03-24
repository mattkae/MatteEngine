#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <random>
#include <chrono>
#include <cmath>

using namespace std;

static const float SKEW_FACTOR = (sqrt(3.f) - 1.f) / 2.f;
static const float UNSKEW_FACTOR = (3.f - sqrt(3.f)) / 6.f;

static inline int32_t fast_floor(float fp) {
    int32_t i = static_cast<int32_t>(fp);
    return (fp < i) ? (i - 1) : (i);
}

static int gradients2D[] = {
  5,  2,    2,  5,
  -5,  2,   -2,  5,
  5, -2,    2, -5,
  -5, -2,   -2, -5,
};

static float grad(glm::vec2 in, glm::vec2 distance, const int* perm)
{
  int hash = perm[(perm[(int)in.x & 0xFF] + (int)in.y) & 0xFF] & 0x0E;
  return gradients2D[hash] * distance.x + gradients2D[hash + 1] * distance.y;
}

static float simplex(glm::vec2 in, const int* perm) {
  float sample = 0; // Resultant value
  
  glm::vec2 s0 = in + (in.x + in.y) * SKEW_FACTOR; // in point in Simplex-Space
  glm::vec2 v0 = glm::vec2(fast_floor(s0.x), fast_floor(s0.y)); // Lower left point in Simplex-Space
  
  glm::vec2 p0 = v0 - (v0.x + v0.y) * UNSKEW_FACTOR; // Lower left point in IN-space
  glm::vec2 d0 = in - p0; // Distance between in point and lower left point, in IN-space

  glm::vec2 v1 = (d0.x > d0.y) ? glm::vec2(1.f, 0.f) : glm::vec2(0.f, 1.f); // Upper left point, or lower right point

  // https://github.com/WardBenjamin/SimplexNoise/blob/master/SimplexNoise/Noise.cs
  glm::vec2 d1 = d0 - v1 + UNSKEW_FACTOR;
  glm::vec2 d2 = d0 - 1.f + 2.f * UNSKEW_FACTOR;

  // Sample from bottom left point
  float sample0 = 0;
  float t0 = 2.f - (d0.x * d0.x) - (d0.y * d0.y);
  if (t0 > 0) {
    t0 *= t0;
    sample0 = t0 * t0 * grad(v0, d0, perm);
  }

  // Sample from upper left/bottom right point
  float sample1 = 0;
  float t1 = 2.f - (d1.x * d1.x) - (d1.y * d1.y);
  if (t1 > 0) {
    t1 *= t1;
    sample1 = t1 * t1 * grad(v0 + v1, d1, perm);
  }

  // Sample from upper right point
  float sample2 = 0;
  float t2 = 2.f - (d2.x * d2.x) - (d2.y * d2.y);
  if (t2 > 0) {
    t2 *= t2;
    sample2 = t2 * t2 * grad(v0 + glm::vec2(1, 1), d2, perm);
  }

  // Adjust sample appropriately
  return (sample0 + sample1 + sample2) / 47.f;
}

Terrain generate_terrain(int dimension, int granularity) {
  Terrain terrain;
  glGenVertexArrays(1, &terrain.vao);
  glGenBuffers(1, &terrain.vbo);
  glGenBuffers(1, &terrain.ebo);
  glBindVertexArray(terrain.vao);

  float squareSize = ((float)dimension / (float)granularity) / 2.f;
  float vertexArrySize = 6 * granularity * granularity;
  float indexArrySize = 6 * granularity * granularity;

  // Generate vertices
  int index = 0;
  GLfloat* vertices = new GLfloat[vertexArrySize];
  int halfGranularity = granularity / 2;
  for (int r = -halfGranularity; r < halfGranularity; r++) {
    for (int c = -halfGranularity; c < halfGranularity; c++) {
      vertices[index] = c * squareSize;
      vertices[index + 1] = 0;
      vertices[index + 2] = r * squareSize;

      index += 6;
    }
  }

  // Generate height map
  float scaleFactor = 1.f / ((float)halfGranularity * (float)squareSize);
  index = 1;
  float max = 15.f;
  
  int* perm = new int[512];
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  for (int p = 0; p < 512; p++) {
    perm[p] = generator();
  }
    
  for (int r = 0; r < granularity; r++) {
    for (int c = 0; c < granularity; c++) {
      glm::vec2 in = glm::vec2(vertices[index - 1], vertices[index + 1]);

      // Add successively smaller, higher-frequency terms
      float frequency = scaleFactor;
      float  amp = 1;
      float  maxAmp = 0;
      float noise = 0;
      for (int i = 0; i < 4; i++) {
        noise += (simplex(frequency * in, perm) * amp);
	maxAmp += amp;
	amp *= 0.5f;
	frequency *= 2.f;
      }

      // Take the averafe values of the iterations
      noise /= maxAmp;

      // Normalize the result
      vertices[index] = noise * (max + max) / 2.f  + (max - max) / 2.f;

      glm::vec3 color;
      if (vertices[index] > max * (3.f / 4.f)) {
	color = glm::vec3(1.0);
      } else if (vertices[index] > max * 0.5f) {
	color = glm::vec3(0.84, 0.74, 0.84);
      } else if (vertices[index] > 0.f) {
	color = glm::vec3(0.0, 0.5, 0.1);
      } else if (vertices[index] > (-max) * 0.25f){
	color = glm::vec3(0.925, 0.78, 0.68);
      } else {
	color = glm::vec3(0.1, 0.7, 0.1);
      }

      vertices[index + 2] = color.x;
      vertices[index + 3] = color.y;
      vertices[index + 4] = color.z;

      index += 6;
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
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*) 0);

  // Color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 6, (GLvoid*) (3 * sizeof(GLfloat)));
  
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
  shader.SetUniform1i("uShowWireframe", terrain.wireframeMode);

  glBindVertexArray(terrain.vao);
  glDrawElements(GL_TRIANGLES, terrain.numIndices, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  
  glEnable(GL_DEPTH);
  glDisable(GL_BLEND);
}
