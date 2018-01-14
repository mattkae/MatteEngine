#include "LightSystem.h"
#include "Shader.h"
#include "Logger.h"
#include <sstream>
#include <string>

using namespace std;

LightSystem::LightSystem() {
  
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
