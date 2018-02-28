#include "Camera.h"
#include "Shader.h"
#include "Input.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace std;

const glm::vec3 WORLD_UP(0.0, 1.0, 0.0);

Camera::Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 right) {
  mPos = pos;
  mUp = up;
  mRight = right;
  mForward = glm::normalize(glm::cross(up, right));
}

void Camera::set_movement_flag(MovementFlag flag) {
  mMovementFlags = mMovementFlags | flag;
}

void Camera::update(double dt) {
  if (mMovementFlags & MovementFlag::MoveForward) {
    mPos += ((float) dt) * mMs.speed * mForward;
  }
  if (mMovementFlags & MovementFlag::MoveBackward) {
    mPos -= ((float) dt) * mMs.speed * mForward;
  }
  if (mMovementFlags & MovementFlag::MoveRight) {
    mPos += ((float) dt) * mMs.speed * mRight;
  }
  if (mMovementFlags & MovementFlag::MoveLeft) {
    mPos -= ((float) dt) * mMs.speed * mRight;
  }
  if (mMovementFlags & MovementFlag::PlusPitch) {
    mVs.pitch += dt * mMs.sensitivity;
    
    if (mVs.pitch > mVs.maxPitch) {
      mVs.pitch = mVs.maxPitch;
    }
  }
  if (mMovementFlags & MovementFlag::MinusPitch) {
    mVs.pitch -= dt * mMs.sensitivity;

    if (mVs.pitch < -mVs.maxPitch) {
      mVs.pitch = -mVs.maxPitch;
    }
  }
  if (mMovementFlags & MovementFlag::PlusYaw) {
    mVs.yaw += dt * mMs.sensitivity;
  }
  if (mMovementFlags & MovementFlag::MinusYaw) {
    mVs.yaw -= dt * mMs.sensitivity;
  }
  
  mMovementFlags = 0;

  glm::vec3 frontTemp;
  float pitch = mVs.pitch;
  float yaw = mVs.yaw - 90.f;
  frontTemp.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
  frontTemp.y = sin(glm::radians(pitch));
  frontTemp.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
  
  mForward = glm::normalize(frontTemp);
  mRight = glm::normalize(glm::cross(mForward, WORLD_UP));
  mUp = glm::normalize(glm::cross(mRight, mForward));
}

glm::mat4 Camera::get_view() const {
  return glm::lookAt(mPos, mPos + mForward, mUp);
}

glm::mat4 Camera::get_projection() const {
  return glm::perspective(glm::radians(mPs.fov), mPs.aspectRatio, mPs.near, mPs.far);
}

void Camera::render(const Shader& shader) const {
  shader.SetUniformMatrix4fv("uVp", 1, GL_FALSE, glm::value_ptr(get_projection() * get_view()));
  shader.SetUniform3f("uEye", mPos.x, mPos.y, mPos.z);
}

void move_camera(double dt, Camera* camera) {
  if (camera == nullptr) {
    cerr << "Camera does not exist to be moved." << endl;
    return;
  }

  Input* input = Input::getInstance();

  // Move around
  if (input->is_down(GLFW_KEY_W)) {
    camera->set_movement_flag(MoveForward);
  }
  if (input->is_down(GLFW_KEY_S)) {
    camera->set_movement_flag(MoveBackward);
  }
  if (input->is_down(GLFW_KEY_A)) {
    camera->set_movement_flag(MoveLeft);
  }
  if (input->is_down(GLFW_KEY_D)) {
    camera->set_movement_flag(MoveRight);
  }

  // Look around
  if (input->is_down(GLFW_KEY_UP)) {
    camera->set_movement_flag(PlusPitch);
  }
  if (input->is_down(GLFW_KEY_DOWN)) {
    camera->set_movement_flag(MinusPitch);
  }
  if (input->is_down(GLFW_KEY_LEFT)) {
    camera->set_movement_flag(MinusYaw);
  }
  if (input->is_down(GLFW_KEY_RIGHT)) {
    camera->set_movement_flag(PlusYaw);
  }
}
