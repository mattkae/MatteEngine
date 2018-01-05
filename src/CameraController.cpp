#include "CameraController.h"
#include "Camera.h"
#include "Input.h"
#include "Logger.h"
#include "gl_includes.h"

CameraController::CameraController(Camera* camera) {
  mCamera = camera;
}

void CameraController::update(double dt) {
  if (mCamera == nullptr) {
    logger::log_error("Unattached CameraController.");
    return;
  }

  if (Input::keys_pressed[GLFW_KEY_W]) {
    mCamera->move_forward(dt);
  }
  if (Input::keys_pressed[GLFW_KEY_S]) {
    mCamera->move_backward(dt);
  }
  if (Input::keys_pressed[GLFW_KEY_A]) {
    mCamera->move_left(dt);
  }
  if (Input::keys_pressed[GLFW_KEY_D]) {
    mCamera->move_right(dt);
  }
  
}