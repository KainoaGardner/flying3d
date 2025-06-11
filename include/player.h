#ifndef PLAYER_H
#define PLAYER_H

#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/quaternion.hpp"
#include <GLFW/glfw3.h>
#include <string>

enum Weapons {
  machineGun,
  shotGun,
  homingMissile,
  bombLauncher,
  chargeRifle,
};

struct ShootArgs {
  glm::vec3 bulletPosition;
  glm::vec3 direction;
  float spin;
};

extern const glm::vec3 CAMERA_POSITION;
extern const glm::vec3 CAMERA_FRONT;
extern const glm::vec3 CAMERA_UP;
extern const glm::quat CAMERA_ORIENTATION;

extern const float SPEED;
extern const float MAX_SPEED;
extern const float TURN_SPEED;
extern const float SENSITIVITY;
extern const float FOV;

class Player {
public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;
  bool leftGun = false;

  // int weapon = machineGun;
  int weapon = shotGun;
  float speed;
  float maxSpeed;
  float turnSpeed;
  float sensitivity;
  float shootCounter = 0.0f;

  float fov;

  glm::quat orientation;
  unsigned int viewDirection;

  Player(glm::vec3 positionIn, glm::vec3 worldUpIn, glm::vec3 frontIn,
         glm::quat orientaionIn, float speedIn, float maxSpeedIn,
         float turnSpeedIn, float sensitivityIn, float fovIn);

  glm::mat4 getViewMatrix(glm::vec3 bossPosition);

  void handleKeyboardInput(GLFWwindow *window, float dt);
  void handleMouseInput(float xOffset, float yOffset, bool constrain = true);
  void handleScrollInput(float yOffset, float minFov, float maxFov);

  void update(float dt);

private:
  bool shooting = false;

  void updateCamera();
  void updateCameraMovement(float dt);

  float addSpeed(float currentSpeed, float maxSpeed, float acceleration,
                 float dt);
  float subtractSpeed(float currentSpeed, float brakeStrength, float dt);
  float applyDrag(float currentSpeed, float dragRate, float dt);

  ShootArgs getShootArgs(float bulletSpread);

  void shootBullet();
  void shootMachineGun();
  void shootShotGun();
  void shootHomingMissile();
  void shootBombLauncher();
  void shootChargeRifle();
};

#endif
