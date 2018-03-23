#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

static const float SKEW_FACTOR = (sqrt(3.f) - 1.f) / 2.f;
static const float UNSKEW_FACTOR = (3.f - sqrt(3.f)) / 6.f;

static inline int32_t fast_floor(float fp) {
    int32_t i = static_cast<int32_t>(fp);
    return (fp < i) ? (i - 1) : (i);
}

static float grad(int hash, float x, float y)
{
  int h = hash & 7;      // Convert low 3 bits of hash code
  float u = h < 4 ? x : y;  // into 8 simple gradient directions,
  float v = h < 4 ? y : x;  // and compute the dot product with (x,y).
  return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f*v : 2.0f*v);
}

static float simplex(glm::vec2 in, const int* perm) {
  float sample = 0; // Resultant value
  
  glm::vec2 s0 = in + (in.x + in.y) * SKEW_FACTOR; // in point in Simplex-Space
  glm::vec2 v0 = glm::vec2(fast_floor(s0.x), fast_floor(s0.y)); // Lower left point in Simplex-Space
  
  glm::vec2 p0 = v0 - (v0.x + v0.y) * UNSKEW_FACTOR; // Lower left point in IN-space
  glm::vec2 d0 = in - p0; // Distance between in point and lower left point, in IN-space

  glm::vec2 v1 = (d0.x > d0.y) ? glm::vec2(1.f, 0.f) : glm::vec2(0.f, 1.f); // Upper left point, or lower right point

  // Logic uncovered from: https://github.com/WardBenjamin/SimplexNoise/blob/master/SimplexNoise/Noise.cs
  // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
  // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
  // c = (3-sqrt(3))/6
  glm::vec2 d1 = d0 - v1 + UNSKEW_FACTOR;
  glm::vec2 d2 = d0 - 1.f + 2.f * UNSKEW_FACTOR;

  glm::vec2 ii = glm::vec2((int)v0.x & 255, (int)v0.y & 255); // Capped value at 256

  // Sample from bottom left point
  float sample0 = 0;
  float t0 = 0.5f - (d0.x * d0.x) - (d0.y * d0.y);
  if (t0 > 0) {
    t0 *= t0;
    sample0 = t0 * t0 * grad(perm[(int)ii.x + perm[(int)ii.y]], d0.x, d0.y);
  }

  // Sample from upper left/bottom right point
  float sample1 = 0;
  float t1 = 0.5f - (d1.x * d1.x) - (d1.y * d1.y);
  if (t1 > 0) {
    t1 *= t1;
    sample1 = t1 * t1 * grad(perm[(int)ii.x + (int)v1.x + perm[(int)ii.y + (int)v1.y]], d1.x, d1.y);
  }

  // Sample from upper right point
  float sample2 = 0;
  float t2 = 0.5f - (d2.x * d2.x) - (d2.y * d2.y);
  if (t2 > 0) {
    t2 *= t2;
    sample2 = t2 * t2 * grad(perm[(int)ii.x + 1 + perm[(int)ii.y + 1]], d1.x, d2.y);
  }

  // Adjust sample appropriately
  return 40.0f * (sample0 + sample1 + sample2);
}

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
  int halfGranularity = granularity / 2;
  for (int r = -halfGranularity; r < halfGranularity; r++) {
    for (int c = -halfGranularity; c < halfGranularity; c++) {
      vertices[index] = c * squareSize;
      vertices[index + 1] = 0;
      vertices[index + 2] = r * squareSize;

      index += 3;
    }
  }

  // Generate height map
  float scaleFactor = 1.f / ((float)halfGranularity * (float)squareSize);
  index = 1;
  float max = 10.f;
  
  int* perm = new int[512];
  srand(time(NULL));
  for (int p = 0; p < 512; p++) {
    perm[p] = rand() % 256;
  }
    
  for (int r = 0; r < granularity; r++) {
    for (int c = 0; c < granularity; c++) {
      glm::vec2 in = glm::vec2(vertices[index - 1], vertices[index + 1]);

      // Add successively smaller, higher-frequency terms
      float frequency = scaleFactor;
      float  amp = 1;
      float  maxAmp = 0;
      float noise = 0;
      for (int i = 0; i < 16; i++) {
        noise += (simplex(frequency * in, perm) * amp);
	maxAmp += amp;
	amp *= 0.5f;
	frequency *= 1.8f;
      }

      // Take the averafe values of the iterations
      noise /= maxAmp;

      // Normalize the result
      vertices[index] = noise * (max + max) / 2.f  + (max - max) / 2.f;
      
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
