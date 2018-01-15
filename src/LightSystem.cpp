#include "LightSystem.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Logger.h"
#include <glm/gtc/matrix_transform.hpp>
#include <sstream>
#include <string>

using namespace std;

LightSystem::LightSystem() {
  // Set up FrameBuffer for shadows
  glGenTextures(1, &mDepthTexture);
  glBindTexture(GL_TEXTURE_2D, mDepthTexture);
  // TO-DO: Don't hardcode values!
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glBindTexture(GL_TEXTURE_2D, 0);

  glGenFramebuffers(1, &mDepthFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, mDepthTexture, 0);
  glDrawBuffer(GL_NONE);
}

static string get_location(int lightIndex, const char* propertyName) {
  ostringstream ss;
  ss << "lights[" << lightIndex << "]." << propertyName;
  string uniformName = ss.str();
  return uniformName.c_str();
}

void LightSystem::render(Shader* shader) {
  // Ambient
  shader->SetUniform3f("ambient", mAmbient.x, mAmbient.y, mAmbient.z);

  // Lights
  shader->SetUniform1i("numLights", mNumLights);
  for (int index = 0; index < mNumLights; index++) {
    Light light = mLights[index];

    if (light.isOn) {
      shader->SetUniform1ui(get_location(index, "type").c_str(), light.type);
      shader->SetUniform3f(get_location(index, "color").c_str(), light.color.x, light.color.y, light.color.z);
      
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

void LightSystem::render_shadows(Shader* shader, Model* model, Camera* camera) {
  // Remember original position and forward
  glm::vec3 origPos = camera->get_position();
  glm::vec3 origForward = camera->get_forward();

  for (int index = 0; index < mNumLights; index++) {
    const Light& light = mLights[mNumLights];

    if (!light.isOn) continue;
    
    CameraSpec spec = camera->get_camera_spec();
    switch(light.type) {
    case Directional:
      camera->set_lookat(-light.direction * spec.far, light.direction);
      break;
    case Point:
      //camera->set_lookat(light.position, model->position - light.position);
      break;
    case Spot:
      //camera->set_lookat(light.position, light.direction);
      break;
    default:
      break;
    }
    camera->render(shader);
    model->render(shader);
  }

  // Reset the camera's position
  camera->set_lookat(origPos, origForward);
}

void LightSystem::set_ambient(glm::vec3 ambient) {
  mAmbient = ambient;
}

int LightSystem::add_directional(glm::vec3 direction, glm::vec3 color) {
  mLights[mNumLights].direction = direction;
  mLights[mNumLights].color = color;
  mLights[mNumLights].type = Directional;

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
