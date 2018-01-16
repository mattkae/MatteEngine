#ifndef CAMERA_H
#define CAMERA_H
#include "Updateable.h"
#include <glm/glm.hpp>

struct CameraSpec {
  // for projection
  float fov = 45.f;
  float width = 800;
  float height = 600;
  float near = 0.1f;
  float far = 100.f;

  // for view
  float cameraSpeed = 5.f;
  float sensitivity = 180.f;
  float defaultPitch = 0.f;
  float defaultYaw = -90.f;
  float maxYaw = 75.f;
  float pitchOffset = 0.f;
  float yawOffset = 0.f;
};

class Shader;

class Camera : public Updateable {
 public:
  Camera();
  Camera(glm::vec3 pos);
  Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 right);
  glm::vec3 get_up() { return mUp; };
  glm::vec3 get_right() { return mRight; };
  glm::vec3 get_forward() { return mForward; };
  glm::vec3 get_position() { return mPos; };
  glm::mat4 get_projection() { return mProjection; };
  glm::mat4 get_view() { return mView; };
  
  CameraSpec get_camera_spec() const { return mSpec; };
  void set_camera_spec(CameraSpec newSpec) { mSpec = newSpec; };
  
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

  bool mViewNeedsUpdate = false;
  bool mVectorsNeedUpdate = false;
};

void move_camera(double dt, Camera* camera);

#endif
