#ifndef CAMERA_H
#define CAMERA_H

#include "../include/glm/glm.hpp"
#include <string>

enum moveDirection {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
};

extern const glm::vec3 CAMERA_POSITION;
extern const glm::vec3 CAMERA_FRONT;
extern const glm::vec3 CAMERA_UP;

extern const float YAW;
extern const float PITCH;
extern const float SPEED;
extern const float SENSITIVITY;
extern const float FOV;

class Camera {
public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw;
  float pitch;
  float speed;
  float sensitivity;
  float fov;

  Camera(glm::vec3 positionIn, glm::vec3 frontIn, glm::vec3 upIn, float yawIn,
         float pitchIn, float speedIn, float sensitivityIn, float fovIn);

  glm::mat4 getViewMatrix();

  void handleKeyboardInput(moveDirection direction, float dt,
                           bool shift = false);
  void handleMouseInput(float xOffset, float yOffset, bool constrain = true);
  void handleScrollInput(float yOffset, float minFov, float maxFov);

private:
  void updateCamera();
};

#endif
