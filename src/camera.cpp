#define GLM_ENABLE_EXPERIMENTAL
#include "../include/camera.h"
#include "../include/bullet.h"
#include "../include/glm/gtx/quaternion.hpp"
#include "../include/glm/gtx/string_cast.hpp"
#include "../include/key.h"
#include <iostream>

const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 CAMERA_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 CAMERA_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);

const float SPEED = 1.0f;
const float MAX_SPEED = 100.0f;
const float TURN_SPEED = 100.0f;
const float SENSITIVITY = 0.2f;
const float FOV = 45.0f;

const float DRAG_RATE = 0.2f;
const float BRAKE_STRENGTH = 0.5f;

const glm::quat CAMERA_ORIENTATION = glm::quat(1, 0, 0, 0);

Camera::Camera(glm::vec3 positionIn, glm::vec3 worldUpIn, glm::vec3 frontIn,
               glm::quat orientationIn, float speedIn, float maxSpeedIn,
               float turnSpeedIn, float sensitivityIn, float fovIn) {
  position = positionIn;
  worldUp = worldUpIn;
  front = frontIn;
  orientation = orientationIn;
  speed = speedIn;
  maxSpeed = maxSpeedIn;
  turnSpeed = turnSpeedIn;
  sensitivity = sensitivityIn;

  shootCooldown = 0.05f;

  fov = fovIn;
  viewDirection = 1;

  updateCamera();
}

glm::mat4 Camera::getViewMatrix() {
  glm::mat4 view = glm::mat4_cast(glm::conjugate(orientation));
  // model = glm::rotate(model, timePassed * 3.0f,
  //                     glm::normalize(glm::vec3(2.0f, 0.0f, 1.0)));

  glm::vec3 cameraUp = glm::normalize(orientation * CAMERA_UP);
  if (viewDirection == 2) {
    view = glm::rotate(view, float(M_PI), cameraUp);
  }
  if (viewDirection == 3) {
    view = glm::rotate(view, float(M_PI / 2), cameraUp);
  }
  if (viewDirection == 4) {
    view = glm::rotate(view, float(-M_PI / 2), cameraUp);
  }

  view = glm::translate(view, -position);

  return view;
}

void Camera::handleKeyboardInput(GLFWwindow *window, float dt) {

  float cameraSpeed = turnSpeed * dt;
  float xOffset = 0.0f;
  float yOffset = 0.0f;
  float zOffset = 0.0f;

  if (glfwGetKey(window, FORWARD_KEY) == GLFW_PRESS) {
    speed += addSpeed(speed, maxSpeed, 0.5f, dt);
  }

  if (glfwGetKey(window, BACKWARD_KEY) == GLFW_PRESS) {
    speed += subtractSpeed(speed, BRAKE_STRENGTH, dt);
    if (speed < 0.0f) {
      speed = 0.0f;
    }
  }

  if (glfwGetKey(window, PITCH_UP_KEY) == GLFW_PRESS) {
    yOffset += cameraSpeed;
  }
  if (glfwGetKey(window, PITCH_DOWN_KEY) == GLFW_PRESS) {
    yOffset -= cameraSpeed;
  }

  if (glfwGetKey(window, ROLL_RIGHT_KEY) == GLFW_PRESS) {
    zOffset += cameraSpeed;
  }
  if (glfwGetKey(window, ROLL_LEFT_KEY) == GLFW_PRESS) {
    zOffset -= cameraSpeed;
  }

  if (glfwGetKey(window, YAW_LEFT_KEY) == GLFW_PRESS) {
    xOffset -= cameraSpeed * 0.5f;
  }
  if (glfwGetKey(window, YAW_RIGHT_KEY) == GLFW_PRESS) {
    xOffset += cameraSpeed * 0.5f;
  }

  if (glfwGetKey(window, SHOOT_KEY) == GLFW_PRESS) {
    shootBullet();
  }

  if (glfwGetKey(window, CAMERA_BACK_KEY) == GLFW_PRESS) {
    viewDirection = 2;
  } else if (glfwGetKey(window, CAMERA_RIGHT_KEY) == GLFW_PRESS) {
    viewDirection = 3;
  } else if (glfwGetKey(window, CAMERA_LEFT_KEY) == GLFW_PRESS) {
    viewDirection = 4;
  } else {
    viewDirection = 1;
  }

  float yawAngle = -xOffset * cameraSpeed;
  float pitchAngle = -yOffset * cameraSpeed * 0.5;
  float rollAngle = zOffset * cameraSpeed;

  glm::vec3 cameraFront = glm::normalize(orientation * CAMERA_FRONT);
  glm::vec3 cameraRight = glm::normalize(orientation * CAMERA_RIGHT);
  glm::vec3 cameraUp = glm::normalize(orientation * CAMERA_UP);

  glm::quat qYaw = glm::angleAxis(glm::radians(yawAngle), cameraUp);
  glm::quat qPitch = glm::angleAxis(glm::radians(pitchAngle), cameraRight);
  glm::quat qRoll = glm::angleAxis(glm::radians(rollAngle), cameraFront);

  orientation = glm::normalize(qRoll * qYaw * qPitch * orientation);

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

// void Camera::handleScrollInput(float yOffset, float minFov, float maxFov) {
//   fov -= yOffset;
//
//   if (fov > maxFov) {
//     fov = maxFov;
//   }
//   if (fov < minFov) {
//     fov = minFov;
//   }
// }

void Camera::update(float dt) {
  updateCameraMovement(dt);
  shootCounter += dt;
}

void Camera::updateCamera() {
  front = glm::normalize(orientation * CAMERA_FRONT);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

void Camera::updateCameraMovement(float dt) {
  front = glm::normalize(orientation * CAMERA_FRONT);

  speed += applyDrag(speed, DRAG_RATE, dt);

  fov = 45.0f + (speed / maxSpeed) * 20.0f;

  float vel = speed;

  position += front * dt * vel;
}

float Camera::addSpeed(float currentSpeed, float maxSpeed, float acceleration,
                       float dt) {
  float dSpeed = maxSpeed - currentSpeed;
  return acceleration * dSpeed * dt;
}

float Camera::subtractSpeed(float currentSpeed, float brakeStrength, float dt) {
  return -brakeStrength * currentSpeed * dt;
}

float Camera::applyDrag(float currentSpeed, float dragRate, float dt) {
  return -dragRate * currentSpeed * dt;
}

void Camera::shootBullet() {
  if (shootCounter < shootCooldown) {
    return;
  }

  shootCounter = 0.0f;

  glm::vec3 cameraUp = glm::normalize(orientation * CAMERA_UP);
  glm::vec3 cameraRight = glm::normalize(orientation * CAMERA_RIGHT);

  glm::vec3 direction = glm::normalize(orientation * CAMERA_FRONT);
  direction.x += ((float(rand() % 100) / 100.0) - 0.5) / 50.0;
  direction.y += ((float(rand() % 100) / 100.0) - 0.5) / 50.0;
  direction.z += ((float(rand() % 100) / 100.0) - 0.5) / 50.0;
  glm::vec3 scale = glm::vec3(0.25f);

  if (leftGun) {
    leftGun = false;
    Bullet bullet = Bullet(position + cameraUp * -2.0f + cameraRight * -1.0f,
                           direction, direction, scale, 1.0f, 1.0f);
    bullets.push_back(bullet);

  } else {
    leftGun = true;
    Bullet bullet = Bullet(position + cameraUp * -2.0f + cameraRight * 1.0f,
                           -direction, direction, scale, 1.0f, 1.0f);
    bullets.push_back(bullet);
  }
}
