#include "Light.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void allocate(Light* light, int width, int height) {
  // Generate the shadow texture
  glGenTextures(1, &light->shadowTexture);
  glBindTexture(GL_TEXTURE_2D, light->shadowTexture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,  GL_LESS);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Generate the framebuffer for the shadow
  glGenFramebuffers(1, &light->depthFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, light->depthFbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, light->shadowTexture, 0);
  
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    cout << "Shadow framebuffer is not okay for light." << endl;
  } else {
    light->usesShadows = true;
  }
  glDrawBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void gen_shadow_texture(Shader* shader, Light light, int width, int height) {
  glBindFramebuffer(GL_FRAMEBUFFER, light.depthFbo);
  glViewport(0, 0, width, height);
  glClearDepth(1.0);
  glClear(GL_DEPTH_BUFFER_BIT);
}


static string get_location(int lightIndex, const char* propertyName) {
  ostringstream ss;
  ss << "u_lights[" << lightIndex << "]." << propertyName;
  string uniformName = ss.str();
  return uniformName;
}

static string get_shadow_location(int lightIndex) {
  ostringstream ss;
  ss << "u_shadowMatrix[" << lightIndex << "]";
  return ss.str();
}

void render_light(Shader* shader, Light light, int index) {
  if (!light.isOn) {
    return;
  }

  shader->SetUniform3f(get_location(index, "color").c_str(), light.color.x, light.color.y, light.color.z);

  if (light.usesShadows) {
    shader->SetUniformMatrix4fv(get_shadow_location(index).c_str(), 1, GL_FALSE, glm::value_ptr(light.projection * light.view));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, light.shadowTexture);
  }
                
  switch (light.type) {
  case Directional:
    shader->SetUniform3f(get_location(index, "direction").c_str(), light.direction.x, light.direction.y, light.direction.z);
    shader->SetUniform3f(get_location(index, "position").c_str(), 0, 0, 0);
    shader->SetUniform1f(get_location(index, "constant").c_str(), 1);
    shader->SetUniform1f(get_location(index, "linear").c_str(), 0);	
    shader->SetUniform1f(get_location(index, "quadratic").c_str(), 0);
    shader->SetUniform1f(get_location(index, "cosineCutOff").c_str(), 0);
    shader->SetUniform1f(get_location(index, "dropOff").c_str(), 1);
    break;
  case Point:
    shader->SetUniform3f(get_location(index, "direction").c_str(), 0, 0, 0);
    shader->SetUniform3f(get_location(index, "position").c_str(), light.position.x, light.position.y, light.position.z);
    shader->SetUniform1f(get_location(index, "constant").c_str(), light.constant);
    shader->SetUniform1f(get_location(index, "linear").c_str(), light.linear);	
    shader->SetUniform1f(get_location(index, "quadratic").c_str(), light.quadratic);
    shader->SetUniform1f(get_location(index, "cosineCutOff").c_str(), 0);
    shader->SetUniform1f(get_location(index, "dropOff").c_str(), 1);
    break;
  case Spot:
    shader->SetUniform3f(get_location(index, "direction").c_str(), light.direction.x, light.direction.y, light.direction.z);
    shader->SetUniform3f(get_location(index, "position").c_str(), light.position.x, light.position.y, light.position.z);
    shader->SetUniform1f(get_location(index, "constant").c_str(), light.constant);
    shader->SetUniform1f(get_location(index, "linear").c_str(), light.linear);	
    shader->SetUniform1f(get_location(index, "quadratic").c_str(), light.quadratic);
    shader->SetUniform1f(get_location(index, "cosineCutOff").c_str(), light.cosineCutOff);
    shader->SetUniform1f(get_location(index, "dropOff").c_str(), light.dropOff);
    break;
  default:
    cerr << "Unknown light type: " << light.type << endl;
    break;
  }
}
