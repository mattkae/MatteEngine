#include "LightSystem.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Logger.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string>

using namespace std;

const glm::mat4 OFFSET_MAT = glm::mat4(
      glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
      glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
      glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
      glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    

LightSystem::LightSystem() {  
  glGenTextures(1, &mDepthTexture);
  glBindTexture(GL_TEXTURE_2D, mDepthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, 800, 600, 0,GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenFramebuffers(1, &mDepthFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    logger::log_error("Shadow framebuffer is not okay.");
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightSystem::render_shadows(Shader* shader, std::vector<Model*> models, Camera* camera) {
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);
  glViewport(0, 0, 800, 600);
  glClearDepth(1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);
  
  shader->Use();
  for (int lightIndex = 0; lightIndex < mNumLights; lightIndex++) {
    const Light& light = mLights[lightIndex];
    if (!light.isOn) continue;

    shader->SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(light.view));
    shader->SetUniformMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(light.projection));

    // Render all of the models
    for (auto model : models) {      
      model->render(shader);
    }
  }

  glDisable(GL_POLYGON_OFFSET_FILL);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static string get_location(int lightIndex, const char* propertyName) {
  ostringstream ss;
  ss << "lights[" << lightIndex << "]." << propertyName;
  string uniformName = ss.str();
  return uniformName.c_str();
}

void LightSystem::render(Shader* shader) {
  //shader->SetUniform1i("depthTexture", 0);
  //glEnable(GL_TEXTURE_2D);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, mDepthTexture);;

  // Ambient
  shader->SetUniform3f("ambient", mAmbient.x, mAmbient.y, mAmbient.z);

  // Lights
  shader->SetUniform1i("numLights", mNumLights);
  for (int index = 0; index < mNumLights; index++) {
    Light light = mLights[index];

    if (light.isOn) {
      shader->SetUniform1ui(get_location(index, "type").c_str(), light.type);
      shader->SetUniform3f(get_location(index, "color").c_str(), light.color.x, light.color.y, light.color.z);
      shader->SetUniformMatrix4fv("shadowMatrix", 1, GL_FALSE, glm::value_ptr(light.projection * light.view * OFFSET_MAT));
      //shader->SetUniformMatrix4fv(get_location(index, "view").c_str(), 1, GL_FALSE, glm::value_ptr(light.view));
      
      switch (light.type) {
      case Directional:
	shader->SetUniform3f(get_location(index, "direction").c_str(), light.direction.x, light.direction.y, light.direction.z);
	break;
      case Spot:
	shader->SetUniform1f(get_location(index, "cosineCutOff").c_str(), light.cosineCutOff);
	shader->SetUniform1f(get_location(index, "dropOff").c_str(), light.dropOff);
	shader->SetUniform3f(get_location(index, "direction").c_str(), light.direction.x, light.direction.y, light.direction.z);
      case Point:
	shader->SetUniform3f(get_location(index, "position").c_str(), light.position.x, light.position.y, light.position.z);
	shader->SetUniform1f(get_location(index, "constant").c_str(), light.constant);
	shader->SetUniform1f(get_location(index, "linear").c_str(), light.linear);	
	shader->SetUniform1f(get_location(index, "quadratic").c_str(), light.quadratic);
	break;
      default:
	logger::log_error("Unknown light type.");
	break;
      }
    } else {
      shader->SetUniform1ui(get_location(index, "type").c_str(), Inactive);
    }
  }
}

void LightSystem::set_ambient(glm::vec3 ambient) {
  mAmbient = ambient;
}

int LightSystem::add_directional(glm::vec3 direction, glm::vec3 color) {
  mLights[mNumLights].direction = direction;
  mLights[mNumLights].color = color;
  mLights[mNumLights].type = Directional;
  mLights[mNumLights].view = glm::lookAt(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0,0,1));
  mLights[mNumLights].projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 1.f, 100.f);

  return mNumLights++;
}

int LightSystem::add_point(glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic) {
  mLights[mNumLights].position = position;
  mLights[mNumLights].color = color;
  mLights[mNumLights].constant = constant;
  mLights[mNumLights].linear = linear;
  mLights[mNumLights].quadratic = quadratic;
  mLights[mNumLights].type = Point;
  
  return mNumLights++;
}


int LightSystem::add_spot(glm::vec3 direction, glm::vec3 position, glm::vec3 color, float constant, float linear, float quadratic, float cosineCutOff, float dropOff) {
  mLights[mNumLights].direction = direction;
  mLights[mNumLights].position = position;
  mLights[mNumLights].color = color;
  mLights[mNumLights].constant = constant;
  mLights[mNumLights].linear = linear;
  mLights[mNumLights].quadratic = quadratic;
  mLights[mNumLights].cosineCutOff = cosineCutOff;
  mLights[mNumLights].dropOff = dropOff;
  mLights[mNumLights].type = Spot;
  
  return mNumLights++;
}

void LightSystem::toggle(int id) {
  if (id < mNumLights) {
    mLights[id].isOn = !mLights[id].isOn;
  }
}
