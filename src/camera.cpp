#define GLM_ENABLE_EXPERIMENTAL
#include "../include/camera.h"
#include "../include/glm/gtx/quaternion.hpp"

const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 CAMERA_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 CAMERA_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);

const float SPEED = 5.0f;
const float TURN_SPEED = 100.0f;
const float SENSITIVITY = 0.2f;
const float FOV = 45.0f;

const glm::quat CAMERA_ORIENTATION = glm::quat(1, 0, 0, 0);

Camera::Camera(glm::vec3 positionIn, glm::vec3 worldUpIn,
                glm::vec3 frontIn,glm::quat orientationIn, float speedIn, float turnSpeedIn,
               float sensitivityIn, float fovIn) {
  position = positionIn;
  worldUp = worldUpIn;
  front = frontIn;
  orientation = orientationIn;
  speed = speedIn;
  turnSpeed = turnSpeedIn;
  sensitivity = sensitivityIn;
  fov = fovIn;
  jets = false;

  updateCamera();
}

glm::mat4 Camera::getViewMatrix() {
  glm::mat4 view = glm::mat4_cast(glm::conjugate(orientation));
  view = glm::translate(view, -position);
  return view;
}

void Camera::handleKeyboardInput(moveDirection direction, float dt,
                                 bool shift) {

  float cameraSpeed = turnSpeed * dt;
  if (shift) {
    jets = true;
    // cameraSpeed *= 2.0f;
  }
  // if (direction == FORWARD) {
  //   position += front * cameraSpeed;
  // }
  // if (direction == BACKWARD) {
  //   position -= front * cameraSpeed;
  // }
  // if (direction == LEFT) {
  //   position -= right * cameraSpeed;
  // }
  // if (direction == RIGHT) {
  //   position += right * cameraSpeed;
  // }


  float xOffset = 0.0f;
  float yOffset = 0.0f;
  float zOffset = 0.0f;

  if (direction == FORWARD && shift != true) {
    yOffset += cameraSpeed;
  }
  if (direction == BACKWARD) {
    yOffset -= cameraSpeed;
  }
  if (direction == LEFT) {
    zOffset += cameraSpeed;
  }
  if (direction == RIGHT) {
    zOffset -= cameraSpeed;
  }


  float yawAngle = -xOffset * cameraSpeed;
  float pitchAngle = -yOffset * cameraSpeed * 0.5;
  float rollAngle = zOffset * cameraSpeed;

  glm::vec3 cameraFront = glm::normalize(orientation * CAMERA_FRONT);
  glm::vec3 cameraRight = glm::normalize(orientation * CAMERA_RIGHT);
  glm::vec3 cameraUp = glm::normalize(orientation *  CAMERA_UP);


  glm::quat qYaw = glm::angleAxis(glm::radians(yawAngle), cameraUp);
  glm::quat qPitch = glm::angleAxis(glm::radians(pitchAngle),cameraRight);
  glm::quat qRoll = glm::angleAxis(glm::radians(rollAngle),cameraFront);

  orientation = glm::normalize(qRoll * qYaw * qPitch  * orientation);

  updateCamera();
}

void Camera::handleMouseInput(float xOffset, float yOffset, bool constrain) {
  // float yawAngle = -xOffset * sensitivity;
  // float pitchAngle = yOffset * sensitivity;
  //
  // glm::quat qYaw = glm::angleAxis(glm::radians(yawAngle), worldUp);
  // glm::vec3 cameraFront = glm::rotate(orientation, CAMERA_FRONT);
  // glm::vec3 cameraRight =
  //     glm::normalize(glm::cross(cameraFront, worldUp));
  //
  // glm::quat qPitch = glm::angleAxis(glm::radians(pitchAngle),cameraRight);
  //
  // orientation = glm::normalize(qYaw * qPitch * orientation);
  //
  // updateCamera();
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
  front = glm::normalize(orientation * CAMERA_FRONT);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

void Camera::updateCameraMovement(float dt){
  front = glm::normalize(orientation * CAMERA_FRONT);

  float vel = 5.0f;
  if (jets){
    vel *= 2.0f;
    jets = false;
  }

  position += front * dt * vel;
}
