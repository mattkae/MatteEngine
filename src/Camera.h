#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

struct CameraSpec {
  float fov = 45.f;
  float pitch = 0;
  float yaw = 0;
  float cameraSpeed = 5.f;
  float sensitivity = 3.f;
};

class Shader;

class Camera {
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
  
  void move_forward();
  void move_backward();
  void move_right();
  void move_left();
  void update_view();
  void update_pitch(bool up);
  void update_yaw(bool right);
  void update_camera_spec(CameraSpec newSpec);
  void render(Shader * shader);
 private:
  // Constants
  const glm::vec3 mWorldUp = glm::vec3(0.0, 1.0, 0.0);

  // Private methods
  void set_projection();
  void update_camera();

  // Orientation of camera
  CameraSpec mSpec;
  glm::vec3 mUp, mRight, mForward, mPos;
  glm::mat4 mProjection;
  glm::mat4 mView;

  bool mViewNeedsUpdate = false;
};

#endif
