#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include "Constants.h"
#include "Scene.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Light get_point(int width, int height) {
  Light pLight;
  glGenTextures(1, &pLight.shadowTexture);
  glBindTexture(GL_TEXTURE_CUBE_MAP, pLight.shadowTexture);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC,  GL_LESS);

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
  pLight.type = Point;
  
  return pLight;
}

Light get_spot(int width, int height) {
  Light sLight;
  glGenTextures(1, &sLight.shadowTexture);
  glBindTexture(GL_TEXTURE_2D, sLight.shadowTexture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,  GL_LESS);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenFramebuffers(1, &sLight.depthFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, sLight.depthFbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sLight.shadowTexture, 0);
  
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    cerr << "Shadow framebuffer is not okay for light." << endl;
  } else {
    sLight.shadowWidth = width;
    sLight.shadowHeight = height;
  }
  glDrawBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  sLight.type = Spot;
  sLight.projection = glm::perspective(glm::radians(45.f), Constants.aspectRatio, Constants.near, Constants.far);
  
  return sLight;
}

Light get_directional(int width, int height) {
  Light dLight;
  glGenTextures(1, &dLight.shadowTexture);
  glBindTexture(GL_TEXTURE_2D, dLight.shadowTexture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,  GL_LESS);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
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

  dLight.type = Directional;
  //dLight.projection = glm::ortho
  
  return dLight;
}


glm::mat4 get_light_view(Light* light) {
  switch (light->type) {
  case Directional:
    return glm::lookAt(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
  case Spot:
    return glm::lookAt(light->position, light->position + light->direction, light->up);
  case Point:
  default:
    cerr << "Attempting to get a view for unknown light: " << light->type << endl;
    return glm::mat4(0.0);
  }
}

void render_point_shadows(Light* light, Shader* shader, Scene* scene) {
  glBindFramebuffer(GL_FRAMEBUFFER, light->depthFbo);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);
  glViewport(0, 0, light->shadowWidth, light->shadowHeight);
  glClearDepth(1.0);

  for (int fidx = 0; fidx < 6; fidx++) {    
    glFramebufferTexture2D(GL_FRAMEBUFFER,
			   GL_DEPTH_ATTACHMENT,
			   GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx,
			   light->shadowTexture, 0);
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

    glm::mat4 view = glm::lookAt(light->position, light->position + currentDirection, up);
    glm::mat4 proj = glm::perspective(glm::radians(90.f), 1.f, Constants.near, Constants.far);
    shader->SetUniformMatrix4fv("uViewProj", 1, GL_FALSE, glm::value_ptr(proj * view));
    scene->render_models(shader);
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glViewport(0, 0, Constants.width, Constants.height);
}

void render_spot_shadows(Light* light, Shader* shader, Scene* scene) {
  glBindFramebuffer(GL_FRAMEBUFFER, light->depthFbo);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);
  glViewport(0, 0, light->shadowWidth, light->shadowHeight);
  glClearDepth(1.0);
  glClear(GL_DEPTH_BUFFER_BIT);

  shader->SetUniformMatrix4fv("uViewProj", 1, GL_FALSE, glm::value_ptr(get_light_view(light) * light->projection));
  scene->render_models(shader);
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glViewport(0, 0, Constants.width, Constants.height);
}

void render_directional_shadows(Light* light, Shader* shader, Scene* scene) {
  glBindFramebuffer(GL_FRAMEBUFFER, light->depthFbo);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);
  glViewport(0, 0, light->shadowWidth, light->shadowHeight);
  glClearDepth(1.0);
  glClear(GL_DEPTH_BUFFER_BIT);

  shader->SetUniformMatrix4fv("uViewProj", 1, GL_FALSE, glm::value_ptr(get_light_view(light) * light->projection));
  scene->render_models(shader);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glViewport(0, 0, Constants.width, Constants.height);
}

void render_shadows_from_light(Light* light, Shader* shader, Scene* scene) {
  if (!light->isOn || !light->usesShadows) return;

  switch (light->type) {
  case Point:
    render_point_shadows(light, shader, scene);
    break;
  case Spot:
    render_spot_shadows(light, shader, scene);
    break;
  case Directional:
    render_directional_shadows(light, shader, scene);
    break;
  default:
    cerr << "Rendering shadows for invalid light type " << light->type << "." << endl;
    break;
  }
}

static string get_array_uniform(int lightIndex, const char* attribute, const char* property = nullptr) {
  ostringstream ss;
  ss << attribute << "[" << lightIndex << "]";

  if (property) {
    ss << "." << property;
  }
  
  return ss.str();
}

void render_point_light(Light* light, Shader* shader, int index) {
  if (light->usesShadows) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, light->shadowTexture);
  }

  shader->SetUniform3f(get_array_uniform(index, "uLights", "direction").c_str(), 0, 0, 0);
  shader->SetUniform3f(get_array_uniform(index, "uLights", "position").c_str(), light->position.x, light->position.y, light->position.z);
  shader->SetUniform1f(get_array_uniform(index, "uLights", "constant").c_str(), light->constant);
  shader->SetUniform1f(get_array_uniform(index, "uLights", "linear").c_str(), light->linear);	
  shader->SetUniform1f(get_array_uniform(index, "uLights", "quadratic").c_str(), light->quadratic);
  shader->SetUniform1f(get_array_uniform(index, "uLights", "cosineCutOff").c_str(), -1);
  shader->SetUniform1f(get_array_uniform(index, "uLights", "dropOff").c_str(), 1);
  
  float near = Constants.near;
  float far = Constants.far;
  float diff = far - near;
  glm::vec2 uFarNear;
  uFarNear.x = (far + near) / diff * 0.5 + 0.5;
  uFarNear.y = -(far * near) / diff;
  shader->SetUniform2f("uFarNear", uFarNear.x, uFarNear.y);
}

void render_directional_light(Light* light, Shader* shader, int index) {
  if (light->usesShadows) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, light->shadowTexture);
    shader->SetUniformMatrix4fv(get_array_uniform(index, "uShadowMatrix").c_str(),
				1, GL_FALSE,
				glm::value_ptr(light->projection * get_light_view(light)));
  }
  
  shader->SetUniform3f(get_array_uniform(index, "uLights", "direction").c_str(), light->direction.x, light->direction.y, light->direction.z);
  shader->SetUniform3f(get_array_uniform(index, "uLights", "position").c_str(), 0, 0, 0);
  shader->SetUniform1f(get_array_uniform(index, "uLights", "constant").c_str(), 1);
  shader->SetUniform1f(get_array_uniform(index, "uLights", "linear").c_str(), 0);	
  shader->SetUniform1f(get_array_uniform(index, "uLights", "quadratic").c_str(), 0);
  shader->SetUniform1f(get_array_uniform(index, "uLights", "cosineCutOff").c_str(), -1);
  shader->SetUniform1f(get_array_uniform(index, "uLights", "dropOff").c_str(), 1);
}

void render_spot_light(Light* light, Shader* shader, int index) {
  if (light->usesShadows) {
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, light->shadowTexture);
    shader->SetUniformMatrix4fv(get_array_uniform(index, "uShadowMatrix").c_str(),
				1, GL_FALSE,
				glm::value_ptr(light->projection * get_light_view(light)));
  }
  
  shader->SetUniform3f(get_array_uniform(index, "uLights", "direction").c_str(), light->direction.x, light->direction.y, light->direction.z);
  shader->SetUniform3f(get_array_uniform(index, "uLights", "position").c_str(), light->position.x, light->position.y, light->position.z);
    shader->SetUniform1f(get_array_uniform(index, "uLights", "constant").c_str(), light->constant);
    shader->SetUniform1f(get_array_uniform(index, "uLights", "linear").c_str(), light->linear);	
    shader->SetUniform1f(get_array_uniform(index, "uLights", "quadratic").c_str(), light->quadratic);
    shader->SetUniform1f(get_array_uniform(index, "uLights", "cosineCutOff").c_str(), light->cosineCutOff);
    shader->SetUniform1f(get_array_uniform(index, "uLights", "dropOff").c_str(), light->dropOff);
}

void render_light(Light* light, Shader* shader, int index) {
  if (!light->isOn) {
    return;
  }

  shader->SetUniform3f(get_array_uniform(index, "uLights", "color").c_str(), light->color.x, light->color.y, light->color.z);
                
  switch (light->type) {
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
    cerr << "Unknown light type: " << light->type << endl;
    break;
  }
}
