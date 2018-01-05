#ifndef CAMERA_CONTROLLER_H
#define CAMERA_CONTROLLER_H
#include "Updateable.h"

class Camera;

class CameraController : public Updateable {
 public:
  CameraController(Camera* camera);
  void update(double dt);
 private:
  Camera* mCamera;
};

#endif
