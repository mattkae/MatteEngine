#ifndef CAMERA_H
#define CAMERA_H
#include "Updateable.h"
#include <glm/glm.hpp>

struct CameraSpec {
  float fov = 45.f;
  float cameraSpeed = 5.f;
  float sensitivity = 180.f;

  float defaultPitch = 0.f;
  float defaultYaw = -90.f;
  float maxYaw = 15.f;
  
  float pitchOffset = 0.f;
  float yawOffset = 0.f;
};

class Shader;

class Camera : public Updateable {
 public:
  Camera();
  Camera(glm::vec3 pos);
  Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 right);
  inline glm::vec3 get_up() { return mUp; };
  inline glm::vec3 get_right() { return mRight; };
  inline glm::vec3 get_forward() { return mForward; };
  inline glm::mat4 get_projection() { return mProjection; };
  inline glm::mat4 get_view() { return mView; };
  
  inline CameraSpec get_camera_spec() { return mSpec; };
  void set_camera_spec(CameraSpec newSpec);
  
  void move_forward(double dt);
  void move_backward(double dt);
  void move_right(double dt);
  void move_left(double dt);
  void update_pitch(double dt, bool up);
  void update_yaw(double dt, bool right);

  void update(double dt);
  void render(Shader * shader);
 private:
  // Constants
  const glm::vec3 mWorldUp = glm::vec3(0.0, 1.0, 0.0);

  // Private methods
  void set_projection();
  
  // Orientation of camera
  CameraSpec mSpec;
  glm::vec3 mUp, mRight, mForward, mPos;
  glm::mat4 mProjection;
  glm::mat4 mView;

  bool mViewNeedsUpdate = true;
  bool mVectorsNeedUpdate = true;
};

#endif
