#include "Light.h"
#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Light::Light() {
  // TO-DO: Set up light
}

Light::~Light() {
  // TO-DO: Deallocate shadow and fbo
}

void Light::use_shadows(int width, int height) {
  // Generate the shadow texture
  glGenTextures(1, &mShadowTexture);
  glBindTexture(GL_TEXTURE_2D, mShadowTexture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC,  GL_LESS);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Generate the framebuffer for the shadow
  glGenFramebuffers(1, &mDepthFbo);
  glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mShadowTexture, 0);
  
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    cout << "Shadow framebuffer is not okay for light." << endl;
  } else {
    mUsesShadows = true;
    mShadowWidth = width;
    mShadowHeight = height;
  }
  glDrawBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Light::render_shadows(Shader* shader, std::vector<Model>* models) {
  if (!this->isOn || !this->mUsesShadows) return;
  
  glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);
  glViewport(0, 0, mShadowWidth, mShadowHeight);
  glClearDepth(1.0);
  glClear(GL_DEPTH_BUFFER_BIT);

  for (auto model : *models) {
    glm::mat4 mvp = mProjection * mView * model.get_model();
    shader->SetUniformMatrix4fv("u_mvp", 1, GL_FALSE, glm::value_ptr(mvp));

    model.render(shader);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glViewport(0, 0, 800, 600);

  //mDrawer.render(this->mShadowTexture);
}


static string get_location(int lightIndex, const char* propertyName) {
  ostringstream ss;
  ss << "u_lights[" << lightIndex << "]." << propertyName;
  string uniformName = ss.str();
  return uniformName;
}

static string get_shadow_location(int lightIndex, const char* attribute) {
  ostringstream ss;
  ss << attribute << "[" << lightIndex << "]";
  return ss.str();
}

void Light::render(Shader* shader, int index) {
  if (!this->isOn) {
    return;
  }

  shader->SetUniform3f(get_location(index, "color").c_str(), this->mColor.x, this->mColor.y, this->mColor.z);

  if (this->mUsesShadows) {
    shader->SetUniformMatrix4fv(get_shadow_location(index, "u_shadowMatrix").c_str(), 1, GL_FALSE, glm::value_ptr(this->mProjection * this->mView));

    shader->SetUniform1i(get_shadow_location(index, "u_depthTexture").c_str(), 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->mShadowTexture);
  }
                
  switch (this->mType) {
  case Directional:
    shader->SetUniform3f(get_location(index, "direction").c_str(), this->mDirection.x, this->mDirection.y, this->mDirection.z);
    shader->SetUniform3f(get_location(index, "position").c_str(), 0, 0, 0);
    shader->SetUniform1f(get_location(index, "constant").c_str(), 1);
    shader->SetUniform1f(get_location(index, "linear").c_str(), 0);	
    shader->SetUniform1f(get_location(index, "quadratic").c_str(), 0);
    shader->SetUniform1f(get_location(index, "cosineCutOff").c_str(), 0);
    shader->SetUniform1f(get_location(index, "dropOff").c_str(), 1);
    break;
  case Point:
    shader->SetUniform3f(get_location(index, "direction").c_str(), 0, 0, 0);
    shader->SetUniform3f(get_location(index, "position").c_str(), this->mPosition.x, this->mPosition.y, this->mPosition.z);
    shader->SetUniform1f(get_location(index, "constant").c_str(), this->mConstant);
    shader->SetUniform1f(get_location(index, "linear").c_str(), this->mLinear);	
    shader->SetUniform1f(get_location(index, "quadratic").c_str(), this->mQuadratic);
    shader->SetUniform1f(get_location(index, "cosineCutOff").c_str(), 0);
    shader->SetUniform1f(get_location(index, "dropOff").c_str(), 1);
    break;
  case Spot:
    shader->SetUniform3f(get_location(index, "direction").c_str(), this->mDirection.x, this->mDirection.y, this->mDirection.z);
    shader->SetUniform3f(get_location(index, "position").c_str(), this->mPosition.x, this->mPosition.y, this->mPosition.z);
    shader->SetUniform1f(get_location(index, "constant").c_str(), this->mConstant);
    shader->SetUniform1f(get_location(index, "linear").c_str(), this->mLinear);	
    shader->SetUniform1f(get_location(index, "quadratic").c_str(), this->mQuadratic);
    shader->SetUniform1f(get_location(index, "cosineCutOff").c_str(), this->mCosineCutOff);
    shader->SetUniform1f(get_location(index, "dropOff").c_str(), this->mDropOff);
    break;
  default:
    cerr << "Unknown light type: " << this->mType << endl;
    break;
  }
}

void Light::update_view() {
  switch (this->mType) {
  case Directional:
    mView = glm::lookAt(10.f * (-mDirection), (10.f * (-mDirection))  + mDirection, mUp);
    break;
  case Point:
    mView = glm::lookAt(mPosition, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    break;
  case Spot:
    mView = glm::lookAt(mPosition, glm::vec3(0, 0, 0), mUp);
    break;
  default:
    cerr << "Updating view on unknown type: " << this->mType << "." << endl;
    break;
  }  
}

void Light::update_projection() {
  switch(this->mType) {
  case Directional:
    mProjection = glm::ortho<float>(-10.f, 10, -10.f, 10, 0.1f, 1000.f);
    break;
  case Point:
    mProjection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 1000.0f);
    break;
  case Spot:
    mProjection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 1000.0f);
    break;
  default:
    cerr << "Updating projection on unknown type: " << this->mType << "." << endl;
    break;
  }
}
