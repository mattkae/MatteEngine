#include "Camera.h"
#include "Shader.h"
#include "Application.h"
#include "Logger.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 DEFAULT_RIGHT = glm::vec3(1.0, 0.0, 0.0);
const glm::vec3 DEFAULT_FORWARD = glm::vec3(0.0, 0.0, -1.0);
const glm::vec3 DEFAULT_POSITION = glm::vec3(0.0, 0.0, 10.0);

const float NEAR = 0.1f;
const float FAR = 100.0f;

void Camera::set_projection() {
  mProjection = glm::perspective(glm::radians(mSpec.fov), (float) Application::screenWidth / (float) Application::screenHeight, NEAR, FAR);
}

Camera::Camera() {
  mUp = mWorldUp;
  mRight = DEFAULT_RIGHT;
  mForward = DEFAULT_FORWARD;
  mPos = DEFAULT_POSITION;

  set_projection();
  update(0);
}

Camera::Camera(glm::vec3 pos) {
  mUp = mWorldUp;
  mRight = DEFAULT_RIGHT;
  mForward = DEFAULT_FORWARD;
  mPos = pos;

  set_projection();
  update(0);
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 right) {
  mUp = up;
  mRight = right;
  mForward = glm::normalize(glm::cross(up, right));

  set_projection();
  update(0);
}


/**
   Move the camera in the four directions
 */
void Camera::move_forward(double dt) {
  mPos += ((float) dt) * mSpec.cameraSpeed * mForward;
  mViewNeedsUpdate = true;
}

void Camera::move_backward(double dt) {
  mPos -= ((float) dt) * mSpec.cameraSpeed * mForward;
  mViewNeedsUpdate = true;
}

void Camera::move_right(double dt) {
  mPos += ((float) dt) * mSpec.cameraSpeed * mRight;
  mViewNeedsUpdate = true;
}

void Camera::move_left(double dt) {
  mPos -= ((float) dt) * mSpec.cameraSpeed * mRight;
  mViewNeedsUpdate = true;
}

/**
   Rotate the camera about the right axis in the provided
   direction based on the camera sensitivity

   @param up True if rotating upward
 */
void Camera::update_pitch(double dt, bool up) {
  mSpec.pitchOffset += dt * mSpec.sensitivity * (up ? 1 : -1);

  // Disallow full rotation
  if (mSpec.pitchOffset > mSpec.maxYaw) {
    mSpec.pitchOffset = mSpec.maxYaw; 
  } else if (mSpec.pitchOffset < -mSpec.maxYaw) {
    mSpec.pitchOffset = -mSpec.maxYaw;
  }
  
  mVectorsNeedUpdate = true;
}

/**
   Rotate the camera about the up axis in the provided direction
   based on the camera sensitivity.

   @param right True if rotating towards the right side of the screen.
 */
void Camera::update_yaw(double dt, bool right) {
  mSpec.yawOffset += dt * mSpec.sensitivity * (right ? 1 : -1);

  mVectorsNeedUpdate = true;
}

/**
   Updates the view matrix at the end of an update if the camera
   changed position, or if the camera rotated.
 */
void Camera::update(double dt) {
  if (mVectorsNeedUpdate) {
    glm::vec3 frontTemp;

    float pitch = this->mSpec.defaultPitch + this->mSpec.pitchOffset;
    float yaw = this->mSpec.defaultYaw + this->mSpec.yawOffset;
    
    frontTemp.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    frontTemp.y = sin(glm::radians(pitch));
    frontTemp.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    this->mForward = glm::normalize(frontTemp);
    this->mRight = glm::normalize(glm::cross(this->mForward, this->mWorldUp));
    this->mUp = glm::normalize(glm::cross(this->mRight, this->mForward));

    this->mVectorsNeedUpdate = false;
    this->mViewNeedsUpdate = true;
  }
  
  if (mViewNeedsUpdate) {
    mView = glm::lookAt(mPos, mPos + mForward, mUp);
    mViewNeedsUpdate = false;
  }
}

void Camera::render(Shader* shader) {
  shader->SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
  shader->SetUniformMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(mProjection));
}
