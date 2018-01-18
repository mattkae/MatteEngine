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

const int SHADOW_FACTOR = 2;

LightSystem::LightSystem() {
  // Generate the shadow texture
  glGenTextures(1, &mDepthTexture);
  glBindTexture(GL_TEXTURE_2D, mDepthTexture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,  GL_LESS);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, SHADOW_FACTOR * 800, SHADOW_FACTOR * 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Generate the framebuffer for the shadow
  glGenFramebuffers(1, &mDepthFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);  
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    logger::log_error("Shadow framebuffer is not okay.");
  else
    logger::log_message("Created framebuffer for light depth.");
  
  glDrawBuffer(GL_NONE);
  //glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightSystem::render_shadows(Shader* shader, Model* model, Model* floor) {
  glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);
  glViewport(0, 0, SHADOW_FACTOR * 800, SHADOW_FACTOR * 600);
  glClearDepth(1.0);
  glClear(GL_DEPTH_BUFFER_BIT);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);

  shader->Use();
  
  for (int lightIndex = 0; lightIndex < mNumLights; lightIndex++) {
    if (!mLights[lightIndex].isOn) continue;
    
    shader->SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mLights[lightIndex].view));
    shader->SetUniformMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(mLights[lightIndex].projection));
  
    model->render(shader);
    floor->render(shader);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
  // Reset everything for later drawing...
  glDisable(GL_POLYGON_OFFSET_FILL);
  glViewport(0, 0, 800, 600);
  
  /*
  for (int mx = 0; mx < 800 * SHADOW_FACTOR; mx++) {
    for (int my = 0; my < 600 * SHADOW_FACTOR; my++) {
      GLfloat depth = 0.0f;
      glReadPixels( mx, my, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth ); 
      if (depth < 1)  {

	std::cout << depth << " ";
      }
    }
    //  std::cout << std::endl;
  }
  */
}

static string get_location(int lightIndex, const char* propertyName) {
  ostringstream ss;
  ss << "lights[" << lightIndex << "]." << propertyName;
  string uniformName = ss.str();
  return uniformName.c_str();
}

void LightSystem::render(Shader* shader) {
  // Depth Texture
  shader->SetUniform1i("depthTexture", 0);
  
  // Ambient
  shader->SetUniform3f("ambient", mAmbient.x, mAmbient.y, mAmbient.z);

  // Lights
  shader->SetUniform1i("numLights", mNumLights);
  for (int index = 0; index < mNumLights; index++) {
    Light light = mLights[index];

    if (light.isOn) {
      shader->SetUniform1ui(get_location(index, "type").c_str(), light.type);
      shader->SetUniform3f(get_location(index, "color").c_str(), light.color.x, light.color.y, light.color.z);
      shader->SetUniformMatrix4fv( "shadowMatrixProj", 1, GL_FALSE, glm::value_ptr(light.projection));
      shader->SetUniformMatrix4fv("shadowMatrixView", 1, GL_FALSE, glm::value_ptr(light.view));
      
      // Bind the depth texture
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, mDepthTexture);
            
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
  mLights[mNumLights].view = glm::lookAt(glm::vec3(0, 3, 0), glm::vec3(0, 0, 0), glm::vec3(0,0,-1));
  mLights[mNumLights].projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.f, 5.f);

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
  mLights[mNumLights].cosineCutOff = cos(cosineCutOff);
  mLights[mNumLights].dropOff = dropOff;
  mLights[mNumLights].type = Spot;
  mLights[mNumLights].view = glm::lookAt(position, position + direction, glm::vec3(0.0, 0.0, 1.0));
  mLights[mNumLights].projection = glm::perspective(glm::radians(cosineCutOff), 800.f / 600.f, 0.1f, 1000.f);
  
  return mNumLights++;
}

void LightSystem::toggle(int id) {
  if (id < mNumLights) {
    mLights[id].isOn = !mLights[id].isOn;
  }
}
