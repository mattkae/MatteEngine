/*#include "Smoke.h"
#include "Shader.h"
#include "Camera.h"

void initialize_smoke(Smoke& smoke) {
  glGenVertexArrays(1, &smoke.vao);
  glGenBuffers(1, &smoke.vbo);
  glBindVertexArray(smoke.vao);

  float* points = new float[smoke.numParticles * smoke.numParticles];
  for (int r = 0; r < smoke.numParticles; r++) {
    for (int c = 0; c < smoke.numParticles; c++) {
      points[r * smoke.numParticles + c] = r * c;
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, smoke.vbo);
  glBufferData(GL_ARRAY_BUFFER, smoke.numParticles, points, GL_STATIC_DRAW);
  
  glBindVertexArray(0);
}

void render_smoke(const Smoke& smoke, const Shader& shader, const Camera& camera) {
  camera.render(shader);
  glBindVertexArray(smoke.vao);
  glDrawArrays(GL_POINTS, 0, smoke.numParticles * smoke.numParticles);
  glBindVertexArrays(0);
}
*/
