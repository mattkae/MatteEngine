#include "Skybox.h"
#include <SOIL.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

const GLfloat vertices[] = {
  1.0f, 1.0f,
  1.0f, -1.0f,
  -1.0f, -1.0f,
  -1.0f, 1.0f
};

const GLuint indices[] = {
  0, 1, 3,
  1, 2, 3
};

bool initialize_skybox(Skybox& box, const char** paths) {
  glGenTextures(1, &box.texture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, box.texture);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // Set data from image
  for (int fidx = 0; fidx < 6; fidx++) {
    GLint w, h;
    unsigned char* image = SOIL_load_image(paths[fidx], &w, &h, 0, SOIL_LOAD_RGBA);
    if (!image) {
      cerr << "Unable to load image for skybox from path " << paths[fidx] << endl;
      return false;
    }

    if (w != h) {
      cerr << "Width and height are unequal in skybox: " << w << " != " << h << endl;
      return false;
    }
    
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx, 0,
		 GL_RGBA,
		 w,
		 h, 0,
		 GL_RGBA,
		 GL_UNSIGNED_BYTE,
		 image);

    SOIL_free_image_data(image);
  }
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  // Generate buffers
  glGenVertexArrays(1, &box.vao);
  glGenBuffers(1, &box.vbo);
  glGenBuffers(1, &box.ebo);
  
  glBindVertexArray(box.vao);

  // Vertex data
  glBindBuffer(GL_ARRAY_BUFFER, box.vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);

  // Index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, box.ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

  // Position
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);
  
  glBindVertexArray(0);
  
  return true;
}


void render_skybox(const Skybox& box, const Shader& shader, const Camera& camera) {
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, box.texture);
  glDisable(GL_DEPTH_TEST);

  glm::mat4 projection = glm::inverse(camera.get_projection());
  glm::mat3 view = glm::inverse(glm::mat3(camera.get_view()));
  
  shader.set_uniform_matrix_4fv("uProjection", 1, GL_FALSE, glm::value_ptr(projection));
  shader.set_uniform_matrix_3fv("uView", 1, GL_FALSE, glm::value_ptr(view));

  glBindVertexArray(box.vao);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
  
  glEnable(GL_DEPTH_TEST);
}
