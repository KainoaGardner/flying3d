#include "../include/camera.h"
#include "../include/glm/gtc/matrix_transform.hpp"

const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 CAMERA_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.2f;
const float FOV = 45.0f;

Camera::Camera(glm::vec3 positionIn, glm::vec3 frontIn, glm::vec3 upIn,
               float yawIn, float pitchIn, float speedIn, float sensitivityIn,
               float fovIn) {
  position = positionIn;
  front = frontIn;
  worldUp = upIn;
  yaw = yawIn;
  pitch = pitchIn;
  speed = speedIn;
  sensitivity = sensitivityIn;
  fov = fovIn;

  updateCamera();
}

glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(position, position + front, up);
}

void Camera::handleKeyboardInput(moveDirection direction, float dt,
                                 bool shift) {

  float cameraSpeed = speed * dt;
  if (shift) {
    cameraSpeed *= 2.0f;
  }
  if (direction == FORWARD) {
    position += front * cameraSpeed;
  }
  if (direction == BACKWARD) {
    position -= front * cameraSpeed;
  }
  if (direction == LEFT) {
    position -= right * cameraSpeed;
  }
  if (direction == RIGHT) {
    position += right * cameraSpeed;
  }
}

void Camera::handleMouseInput(float xOffset, float yOffset, bool constrain) {
  xOffset *= sensitivity;
  yOffset *= sensitivity;

  yaw += xOffset;
  pitch += yOffset;

  if (constrain) {
    if (pitch > 89.0f) {
      pitch = 89.0f;
    }
    if (pitch < -89.0f) {
      pitch = -89.0f;
    }
  }

  updateCamera();
}

void Camera::handleScrollInput(float yOffset, float minFov, float maxFov) {
  fov -= yOffset;

  if (fov > maxFov) {
    fov = maxFov;
  }
  if (fov < minFov) {
    fov = minFov;
  }
}

void Camera::updateCamera() {
  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  front = glm::normalize(direction);

  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}
