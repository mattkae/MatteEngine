#include "Light.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

Light::Light(Camera* camera) {
  mCamera = camera;
}

Light::~Light() {
  // TO-DO: Deallocate shadow and fbo
}

void Light::use_shadows(int width, int height) {
  if (mType == Directional || mType == Spot) {
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
  } else if (mType == Point) {
    // Generate the cube map
    glGenTextures(1, &mShadowTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mShadowTexture);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC,  GL_LEQUAL);

    for (int fidx = 0; fidx < 6; fidx++) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx, 0,
		   GL_DEPTH_COMPONENT32,
		   width,
		   height, 0,
		   GL_DEPTH_COMPONENT,
		   GL_FLOAT,
		   0);
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Generate the framebuffer for the shadow
    glGenFramebuffers(1, &mDepthFbo);
    glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);
    glDrawBuffer(GL_NONE);

    mUsesShadows = true;
    mShadowWidth = width;
    mShadowHeight = height;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  } else {
    cerr << "Attempting to create a shadow texture for an undefined light." << endl;
  }
}

void Light::render_shadows(Shader* shader, std::vector<Model>* models) {
  if (!this->isOn || !this->mUsesShadows) return;

  if (mType == Point) {
    glBindFramebuffer(GL_FRAMEBUFFER, mDepthFbo);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(2.0f, 4.0f);
    glViewport(0, 0, mShadowWidth, mShadowHeight);
    glClearDepth(1.0);

    for (int fidx = 0; fidx < 6; fidx++) {
      // Render scene from 90 degree increments around the point light
      glFramebufferTexture2D(GL_FRAMEBUFFER,
			     GL_DEPTH_ATTACHMENT,
			     GL_TEXTURE_CUBE_MAP_POSITIVE_X + fidx,
			     mShadowTexture, 0);
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
	up = glm::vec3(0, -1, 0);
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
	up = glm::vec3(0, -1, 0);
	break;
      case 5: 
	currentDirection = glm::vec3(0, 0, -1);
	up = glm::vec3(0, -1, 0);
	break;
      default:
	cerr << "Invalid direction for cube map." << endl;
	break;
      }
      
      glm::mat4 currentView = glm::lookAt(mPosition, mPosition + currentDirection, up);
      for (auto model : *models) {
	glm::mat4 mvp = mProjection * currentView * model.get_model();
	shader->SetUniformMatrix4fv("u_mvp", 1, GL_FALSE, glm::value_ptr(mvp));
	model.render(shader);
      }
    }
  } else {    
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
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_POLYGON_OFFSET_FILL);
  glViewport(0, 0, 800, 600);
  //  mDrawer.render(this->mShadowTexture);
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

  if (this->mUsesShadows) {
    if (mType == Point) {
      glActiveTexture(GL_TEXTURE0 + index);
      glBindTexture(GL_TEXTURE_CUBE_MAP, this->mShadowTexture);
      shader->SetUniform1i("u_pointDepthTexture", index);
      
      glm::vec3 currentDirection = glm::vec3(0, -1, 0);
      glm::vec3 up = glm::vec3(0.0, 0.0, -1.0);
      glm::mat4 currentView = glm::lookAt(mPosition, mPosition + currentDirection, up);
      shader->SetUniformMatrix4fv(get_shadow_location(index, "u_shadowMatrix").c_str(), 1, GL_FALSE, glm::value_ptr(this->mProjection * currentView));
      shader->SetUniformMatrix4fv(get_location(index, "projection").c_str(), 1, GL_FALSE, glm::value_ptr(this->mProjection));
    } else {
      /*
      glActiveTexture(GL_TEXTURE0 + index);
      glBindTexture(GL_TEXTURE_2D, this->mShadowTexture);
      shader->SetUniform1i(get_shadow_location(index, "u_depthTextures").c_str(), index);
      shader->SetUniformMatrix4fv(get_shadow_location(index, "u_shadowMatrix").c_str(), 1, GL_FALSE, glm::value_ptr(this->mProjection * this->mView));*/
    }
  }

  shader->SetUniform3f(get_location(index, "color").c_str(), this->mColor.x, this->mColor.y, this->mColor.z);
                
  switch (this->mType) {
  case Directional:
    shader->SetUniform3f(get_location(index, "direction").c_str(), this->mDirection.x, this->mDirection.y, this->mDirection.z);
    shader->SetUniform3f(get_location(index, "position").c_str(), 0, 0, 0);
    shader->SetUniform1f(get_location(index, "constant").c_str(), 1);
    shader->SetUniform1f(get_location(index, "linear").c_str(), 0);	
    shader->SetUniform1f(get_location(index, "quadratic").c_str(), 0);
    shader->SetUniform1f(get_location(index, "cosineCutOff").c_str(), -1);
    shader->SetUniform1f(get_location(index, "dropOff").c_str(), 1);
    break;
  case Point:
    shader->SetUniform3f(get_location(index, "direction").c_str(), 0, 0, 0);
    shader->SetUniform3f(get_location(index, "position").c_str(), this->mPosition.x, this->mPosition.y, this->mPosition.z);
    shader->SetUniform1f(get_location(index, "constant").c_str(), this->mConstant);
    shader->SetUniform1f(get_location(index, "linear").c_str(), this->mLinear);	
    shader->SetUniform1f(get_location(index, "quadratic").c_str(), this->mQuadratic);
    shader->SetUniform1f(get_location(index, "cosineCutOff").c_str(), -1);
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
    mView = glm::lookAt(glm::vec3(0, 10, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    break;
  case Spot:
    mView = glm::lookAt(mPosition, mPosition + mDirection, mUp);
    break;
  case Point:
    break;
  default:
    cerr << "Updating view on unknown type: " << this->mType << "." << endl;
    break;
  }  
}

void Light::update_projection() {
  switch(this->mType) {
  case Directional:
    //    mProjection = glm::ortho<float>(-10.f, 10, -10.f, 10, 0.1f, 1000.f);
    mProjection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 1000.0f);
    break;
  case Point:
    mProjection = glm::perspective(glm::radians(90.f), 1.f, 0.1f, 1000.0f);
    break;
  case Spot:
    mProjection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 1000.0f);
    break;
  default:
    cerr << "Updating projection on unknown type: " << this->mType << "." << endl;
    break;
  }
}
