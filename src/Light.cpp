#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "Constants.h"
#include "Scene.h"
#include "ImageUtil.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

void create_shadow_texture_for_ominidirectional_light(Light& pLight, int width, int height) {
  glGenTextures(1, &pLight.shadowTexture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, pLight.shadowTexture);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC,  GL_LEQUAL);

  for (int fidx = 0; fidx < 6; fidx++) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx, 0,
		 GL_DEPTH_COMPONENT24,
		 width,
		 height, 0,
		 GL_DEPTH_COMPONENT,
		 GL_FLOAT,
		 0);
  }

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  // Generate the framebuffer for the shadow
  glGenFramebuffers(1, &pLight.depthFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, pLight.depthFbo);
  glDrawBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  pLight.shadowWidth = width;
  pLight.shadowHeight = height;
}

void create_shadow_texture_for_directional_light(Light& dLight, int width, int height) {
  glGenTextures(1, &dLight.shadowTexture);
  glBindTexture(GL_TEXTURE_2D, dLight.shadowTexture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenFramebuffers(1, &dLight.depthFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, dLight.depthFbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, dLight.shadowTexture, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    cerr << "Shadow framebuffer is not okay for light." << endl;
  } else {
    dLight.shadowWidth = width;
    dLight.shadowHeight = height;
  }

  glDrawBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void create_shadow_texture_for_light(Light& light, int width, int height) {
	switch (light.type) {
	case Spot:
	case Directional:
		create_shadow_texture_for_directional_light(light, width, height);
		break;
	case Point:
		create_shadow_texture_for_ominidirectional_light(light, width, height);
  default:
    break;
	}
}

void render_omindirectional_shadows(const Light& light, Shader& shader, Scene& scene) {
  glBindFramebuffer(GL_FRAMEBUFFER, light.depthFbo);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);
  glViewport(0, 0, light.shadowWidth, light.shadowHeight);
  glClearDepth(1.0);

  for (int fidx = 0; fidx < 6; fidx++) {
    glFramebufferTexture2D(GL_FRAMEBUFFER,
			   GL_DEPTH_ATTACHMENT,
			   GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx,
			   light.shadowTexture, 0);
    glClear(GL_DEPTH_BUFFER_BIT);


    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
      cerr << "Shadow FBO is broken with code " << status << endl;
    }

    glm::vec3 currentDirection;
    glm::vec3 up;
    switch (fidx) {
    case 0:
      currentDirection = glm::vec3(1, 0, 0);
      up = glm::vec3(0, 1, 0);
      break;
    case 1:
      currentDirection = glm::vec3(-1, 0, 0);
      up = glm::vec3(0, -1, 0);
      break;
    case 2:
      currentDirection = glm::vec3(0, 1, 0);
      up = glm::vec3(0.0, 0.0, 1.0);
      break;
    case 3:
      currentDirection = glm::vec3(0, -1, 0);
      up = glm::vec3(0.0, 0.0, -1.0);
      break;
    case 4:
      currentDirection = glm::vec3(0, 0, 1);
      up = glm::vec3(0, 1, 0);
      break;
    case 5:
      currentDirection = glm::vec3(0, 0, -1);
      up = glm::vec3(0, -1, 0);
      break;
    default:
      cerr << "Invalid direction for cube map." << endl;
      break;
    }

    glm::mat4 view = glm::lookAt(light.position, light.position + currentDirection, up);
    glm::mat4 proj = glm::perspective(glm::radians(90.f), 1.f, Constants.near, Constants.far);
    shader.set_uniform_matrix_4fv("uViewProj", 1, GL_FALSE, glm::value_ptr(proj * view));
    scene.render_models(shader);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glViewport(0, 0, Constants.width, Constants.height);
}

void render_directional_shadows(const Light& light, Shader& shader, std::vector<Model>& models) {
	glBindFramebuffer(GL_FRAMEBUFFER, light.depthFbo);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.0f, 4.0f);
	glViewport(0, 0, light.shadowWidth, light.shadowHeight);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);

	shader.set_uniform_matrix_4fv("uView", 1, GL_FALSE, glm::value_ptr(light.view));
	shader.set_uniform_matrix_4fv("uProjection", 1, GL_FALSE, glm::value_ptr(light.projection));
	for (auto model : models) {
		model.render(shader, false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_POLYGON_OFFSET_FILL);
	glViewport(0, 0, Constants.width, Constants.height);
}

void render_shadows_from_light(const Light& light, Shader& shader, std::vector<Model>& models) {
  if (!light.isOn || !light.usesShadows) return;

  switch (light.type) {
  case Point:
    //render_omindirectional_shadows(light, shader, scene);
    break;
  case Spot:
  case Directional:
    render_directional_shadows(light, shader, models);
    break;
  default:
    cerr << "Rendering shadows for invalid light type " << light.type << "." << endl;
    break;
  }
}

static string get_array_uniform(const int lightIndex, const char* attribute, const char* property = nullptr) {
  ostringstream ss;
  ss << attribute << "[" << lightIndex << "]";

  if (property) {
    ss << "." << property;
  }

  return ss.str();
}

void render_point_light(const Light& light, Shader& shader, int index) {
  if (light.usesShadows) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, light.shadowTexture);
  }

  shader.set_uniform_3f(get_array_uniform(index, "uLights", "direction").c_str(), 0, 0, 0);
  shader.set_uniform_3f(get_array_uniform(index, "uLights", "position").c_str(), light.position.x, light.position.y, light.position.z);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "constant").c_str(), light.constant);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "linear").c_str(), light.linear);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "quadratic").c_str(), light.quadratic);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "cosineCutOff").c_str(), -1);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "dropOff").c_str(), 1);

  float near = Constants.near;
  float far = Constants.far;
  float diff = far - near;
  glm::vec2 uFarNear;
  uFarNear.x = (far + near) / diff * 0.5 + 0.5;
  uFarNear.y = -(far * near) / diff;
  shader.set_uniform_2f("uFarNear", uFarNear.x, uFarNear.y);
}

void render_directional_light(const Light& light, Shader& shader, const int index) {
  if (light.usesShadows) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, light.shadowTexture);
    shader.set_uniform_1i("uDirShadow", 0);
    shader.set_uniform_matrix_4fv("uShadowMatrix", 1, GL_FALSE, glm::value_ptr(light.projection * light.view));
  }

  glm::vec3 position = glm::vec3(light.direction);
  position *= -Constants.far;
  shader.set_uniform_3f(get_array_uniform(index, "uLights", "direction").c_str(), light.direction.x, light.direction.y, light.direction.z);
  shader.set_uniform_3f(get_array_uniform(index, "uLights", "position").c_str(), position.x, position.y, position.z);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "constant").c_str(), 1);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "linear").c_str(), 0);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "quadratic").c_str(), 0);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "cosineCutOff").c_str(), -1);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "dropOff").c_str(), 1);
}

void render_spot_light(const Light& light, Shader& shader, const int index) {
  if (light.usesShadows) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, light.shadowTexture);
    shader.set_uniform_1i("uDirShadow", 0);
    shader.set_uniform_matrix_4fv("uShadowMatrix", 1, GL_FALSE, glm::value_ptr(light.projection * light.view));
  }

  shader.set_uniform_3f(get_array_uniform(index, "uLights", "direction").c_str(), light.direction.x, light.direction.y, light.direction.z);
  shader.set_uniform_3f(get_array_uniform(index, "uLights", "position").c_str(), light.position.x, light.position.y, light.position.z);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "constant").c_str(), light.constant);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "linear").c_str(), light.linear);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "quadratic").c_str(), light.quadratic);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "cosineCutOff").c_str(), light.cosineCutOff);
  shader.set_uniform_1f(get_array_uniform(index, "uLights", "dropOff").c_str(), light.dropOff);
}

void render_light(const Light& light, Shader& shader, const int index) {
  if (!light.isOn) {
    return;
  }

  shader.set_uniform_3f(get_array_uniform(index, "uLights", "color").c_str(), light.color.x, light.color.y, light.color.z);

  switch (light.type) {
  case Directional:
    render_directional_light(light, shader, index);
    break;
  case Point:
    render_point_light(light, shader, index);
    break;
  case Spot:
    render_spot_light(light, shader, index);
    break;
  default:
    cerr << "Unknown light type: " << light.type << endl;
    break;
  }
}
