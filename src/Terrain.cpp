#include "Terrain.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

static const float SKEW_FACTOR = (sqrt(3.f) - 1.f) / 2.f;
static const float UNSKEW_FACTOR = (3.f - sqrt(3.f)) / 6.f;

static const unsigned char perm[512] = { 151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
151,160,137,91,90,15,
131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

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


static float simplex(glm::vec2 in) {
  in = in;
  
  float sample = 0;
  glm::vec2 s0 = in + (in.x + in.y) * SKEW_FACTOR; // Simplex space

  // Lower left point
  glm::vec2 i0 = glm::vec2(fast_floor(s0.x), fast_floor(s0.y)); // Simplex space
  glm::vec2 p0 = i0 - (i0.x + i0.y) * UNSKEW_FACTOR;
  glm::vec2 d0 = in - p0;

  glm::vec2 v1 = (d0.x > d0.y) ? glm::vec2(1.f, 0.f) : glm::vec2(0.f, 1.f);

  glm::vec2 d1 = d0 - v1 + UNSKEW_FACTOR;
  glm::vec2 d2 = d0 - 1.f + 2.f * UNSKEW_FACTOR;

  glm::vec2 ii = glm::vec2((int)i0.x & 0xff, (int)i0.y & 0xff);

  float sample0 = 0;
  float t0 = 0.5f - (d0.x * d0.x) - (d0.y * d0.y);
  if (t0 > 0) {
    t0 *= t0;
    sample0 = t0 * t0 * grad(perm[(int)ii.x + perm[(int)ii.y]], d0.x, d0.y);
  }

  float sample1 = 0;
  float t1 = 0.5f - (d1.x * d1.x) - (d1.y * d1.y);
  if (t1 > 0) {
    t1 *= t1;
    sample1 = t1 * t1 * grad(perm[(int)ii.x + (int)i0.x + perm[(int)ii.y + (int)i0.y]], d1.x, d1.y);
  }

  float sample2 = 0;
  float t2 = 0.5f - (d2.x * d2.x) - (d2.y * d2.y);
  if (t2 > 0) {
    t2 *= t2;
    sample2 = t2 * t2 * grad(perm[(int)ii.x + 1 + perm[(int)ii.y + 1]], d1.x, d2.y);
  }
  
  return 5 * (40.0f / 0.884343445f * (sample0 + sample1 + sample2));
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
  index = 1; // Start at y index
  float scaleFactor = 1.f / ((float)halfGranularity * (float)squareSize); 
  for (int r = 0; r < granularity; r++) {
    for (int c = 0; c < granularity; c++) {
      glm::vec2 in = glm::vec2(vertices[index - 1], vertices[index + 1]);
      in *= scaleFactor;
      vertices[index] += simplex(in);
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
