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
const float MAX_YAW_ROTATION = 65.0f;
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

  mViewNeedsUpdate = true;
  set_projection();
  update(0);
}

Camera::Camera(glm::vec3 pos) {
  mUp = mWorldUp;
  mRight = DEFAULT_RIGHT;
  mForward = DEFAULT_FORWARD;
  mPos = pos;

  mViewNeedsUpdate = true;
  set_projection();
  update(0);
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 right) {
  mUp = up;
  mRight = right;
  mForward = glm::normalize(glm::cross(up, right));

  mViewNeedsUpdate = true;
  set_projection();
  update(0);
}

/**
   Update the facing vectors of the camera based on the current
   yaw and pitch
 */
void Camera::update_camera() {
  using namespace std;

  // Calculate the new front facing vector based off of pitch and yaw
  glm::vec3 frontTemp;
  frontTemp.x = cos(glm::radians(mSpec.pitch)) * cos(glm::radians(mSpec.yaw));
  frontTemp.y = sin(glm::radians(mSpec.pitch));
  frontTemp.z = cos(glm::radians(mSpec.pitch)) * sin(glm::radians(mSpec.yaw));

  // Update right and up
  mForward = glm::normalize(frontTemp);
  mRight = glm::normalize(glm::cross(mForward, mWorldUp));
  mUp = glm::normalize(glm::cross(mRight, mForward));

  // Update the view matrix
  mViewNeedsUpdate = true;  
}

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
void Camera::update_pitch(bool up) {
  mSpec.pitch = mSpec.sensitivity * (up ? 1 : -1);

  // Disallow full rotation
  if (mSpec.pitch > MAX_YAW_ROTATION) {
    mSpec.pitch = MAX_YAW_ROTATION; 
  } else if (mSpec.pitch < -MAX_YAW_ROTATION) {
    mSpec.pitch = -MAX_YAW_ROTATION;
  }
  
  update_camera();
}

/**
   Rotate the camera about the up axis in the provided direction
   based on the camera sensitivity.

   @param right
 */
void Camera::update_yaw(bool right) {
  mSpec.yaw += mSpec.sensitivity * (right ? 1 : -1);

  update_camera();
}

/**
   Updates the view matrix at the end of an update if the camera
   changed position, or if the camera rotated.
 */
void Camera::update(double dt) {
  if (mViewNeedsUpdate) {
    mView = glm::lookAt(mPos, mPos + mForward, mUp);
    mViewNeedsUpdate = false;
  }
}

void Camera::render(Shader* shader) {
  shader->SetUniformMatrix4fv("view", 1, GL_FALSE, glm::value_ptr(mView));
  shader->SetUniformMatrix4fv("projection", 1, GL_FALSE, glm::value_ptr(mProjection));
}
