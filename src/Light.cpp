#include "Light.h"
#include "Shader.h"

static const char* get_location(int lightIndex, const char* propertyName) {
  ostringstream ss;
  ss << "lights[" << lightIndex << "]." << propertyName;
  string uniformName = ss.str();
  return uniformName.c_str();
}

void render(Shader* shader) {
  if (isOn) {
      shader->SetUniform1ui(get_location(index, "type"), light.type);
      shader->SetUniform3f(get_location(index, "color"), light.color.x, light.color.y, light.color.z);
      shader->SetUniformMatrix4fv(get_location(index, "shadowMatrix"), 1, GL_FALSE(glm::value_ptr(light.projection * light.view)));
      
      // Bind the depth texture
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, mDepthTexture);
            
      switch (light.type) {
      case Directional:
	shader->SetUniform3f(get_location(index, "direction"),
			     light.direction.x, light.direction.y, light.direction.z);
	break;
      case Spot:
	shader->SetUniform1f(get_location(index, "cosineCutOff"),
			     light.cosineCutOff);
	shader->SetUniform1f(get_location(index, "dropOff"),
			     light.dropOff);
	shader->SetUniform3f(get_location(index, "direction"),
			     light.direction.x, light.direction.y, light.direction.z);
      case Point:
	shader->SetUniform3f(get_location(index, "position"),
			     light.position.x, light.position.y, light.position.z);
	shader->SetUniform1f(get_location(index, "constant"),
			     light.constant);
	shader->SetUniform1f(get_location(index, "linear"),
			     light.linear);	
	shader->SetUniform1f(get_location(index, "quadratic"),
			     light.quadratic);
	break;
      default:
	logger::log_error("Unknown light type.");
	break;
      }
    } else {
      shader->SetUniform1ui(get_location(index, "type"), Inactive);
    }
}
