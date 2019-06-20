#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL

#include "Constants.h"
#include <glm/glm.hpp>

enum MovementFlag {
  MoveForward = 0x01,
  MoveBackward = 0x02,
  MoveRight = 0x04,
  MoveLeft = 0x08,
  PlusPitch = 0x10,
  MinusPitch = 0x20,
  PlusYaw = 0x40,
  MinusYaw = 0x80
};

struct ProjectionSpec {
  float fov    = 60.f;
  float aspectRatio = Constants.aspectRatio;
  float near   = Constants.near;
  float far    = Constants.far;
};

struct ViewSpec {
  float pitch    = 0.f;
  float yaw      = 0.f;
  float maxPitch   = 75.f;
};

struct MovementSpec {
  float speed       = 5.f;
  float sensitivity = 180.f;
};

class Shader;

class Camera {
 public:
  Camera(glm::vec3 pos = glm::vec3(0, 2.5, 10), glm::vec3 up = glm::vec3(0, 1, 0), glm::vec3 right = glm::vec3(1, 0, 0));
  glm::vec3 get_up() const { return mUp; };
  glm::vec3 get_right() const { return mRight; };
  glm::vec3 get_forward() const { return mForward; };
  glm::vec3 get_position() const { return mPos; };
  glm::mat4 get_view() const;
  glm::mat4 get_projection() const;
  void set_position(glm::vec3 pos) { mPos = pos; };

  void set_movement_flag(MovementFlag flag);
  void update(double dt);
  void render(const Shader& shader, bool withEye = true) const;
 private:
  ProjectionSpec mPs;
  ViewSpec mVs;
  MovementSpec mMs;
  glm::vec3 mUp, mRight, mForward, mPos;
  unsigned char mMovementFlags = 0;
};

void move_camera(double dt, Camera* camera);

#endif
