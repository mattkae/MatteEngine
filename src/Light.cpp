#include "Light.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

const int SHADOW_FACTOR = 2;

static const char* get_location(int lightIndex, const char* propertyName) {
  ostringstream ss;
  ss << "lights[" << lightIndex << "]." << propertyName;
  string uniformName = ss.str();
  return uniformName.c_str();
}

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

void render_light(Shader* shader, Light light, int index) {
  if (light.isOn) {
    shader->SetUniform1ui(get_location(index, "type"), Inactive);
    return;
  }
  
  shader->SetUniform3f(get_location(index, "color"), light.color.x, light.color.y, light.color.z);
  shader->SetUniformMatrix4fv(get_location(index, "shadowMatrix"), 1, GL_FALSE, glm::value_ptr(light.projection * light.view));
      
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, light.shadowTexture);
            
  switch (light.type) {
  case Directional:
    shader->SetUniform3f(get_location(index, "direction"), light.direction.x, light.direction.y, light.direction.z);
    shader->SetUniform3f(get_location(index, "position"), 0, 0, 0);
    shader->SetUniform1f(get_location(index, "constant"), 1);
    shader->SetUniform1f(get_location(index, "linear"), 0);	
    shader->SetUniform1f(get_location(index, "quadratic"), 0);
    shader->SetUniform1f(get_location(index, "cosineCutOff"), 0);
    shader->SetUniform1f(get_location(index, "dropOff"), 0);
    break;
  case Spot:
    shader->SetUniform3f(get_location(index, "direction"), light.direction.x, light.direction.y, light.direction.z);
    shader->SetUniform3f(get_location(index, "position"), light.position.x, light.position.y, light.position.z);
    shader->SetUniform1f(get_location(index, "constant"), light.constant);
    shader->SetUniform1f(get_location(index, "linear"), light.linear);	
    shader->SetUniform1f(get_location(index, "quadratic"), light.quadratic);
    shader->SetUniform1f(get_location(index, "cosineCutOff"), light.cosineCutOff);
    shader->SetUniform1f(get_location(index, "dropOff"), light.dropOff);
    break;
  case Point:
    shader->SetUniform3f(get_location(index, "direction"), 0, 0, 0);
    shader->SetUniform3f(get_location(index, "position"), light.position.x, light.position.y, light.position.z);
    shader->SetUniform1f(get_location(index, "constant"), light.constant);
    shader->SetUniform1f(get_location(index, "linear"), light.linear);	
    shader->SetUniform1f(get_location(index, "quadratic"), light.quadratic);
    shader->SetUniform1f(get_location(index, "cosineCutOff"), 0);
    shader->SetUniform1f(get_location(index, "dropOff"), 0);
    break;
  default:
    cerr << "Unknown light type: " << light.type << endl;
    break;
  }
}
