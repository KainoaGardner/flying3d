#ifndef PLAYER_H
#define PLAYER_H

#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/quaternion.hpp"
#include "../include/ships.h"
#include <GLFW/glfw3.h>
#include <string>

enum Weapons {
  machineGun,
  shotGun,
  homingMissile,
  bombLauncher,
  chargeRifle,
};

enum Ships {
  normalShip,
  tankShip,
  timeShip,
  vampireShip,
  speedShip,
  parryShip,

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

extern const float FOV;

extern const float MAX_COUNTER;

class Player {
public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  unsigned int weaponIndex = 0;
  unsigned int ship;

  float speed = 0.0f;
  float fov = 45.0f;

  float damageBoost = 1.0;
  float speedBoost = 1.0;
  float shootSpeedBoost = 1.0;

  glm::quat orientation;
  unsigned int viewDirection;

  Player(glm::vec3 positionIn, glm::vec3 worldUpIn, glm::vec3 frontIn,
         glm::quat orientaionIn, unsigned int shipIn, unsigned int weapons[2]);

  glm::mat4 getViewMatrix(glm::vec3 bossPosition);

  void handleKeyboardInput(GLFWwindow *window, float dt);
  void handleMouseInput(float xOffset, float yOffset, bool constrain = true);
  void handleScrollInput(float yOffset, float minFov, float maxFov);

  void update(float dt);

private:
  bool leftGun = false;
  bool canWeaponSwap = true;

  float shootCounter = 0.0f;
  float abilityCounter = MAX_COUNTER;
  float ultimateCounter = 0.0f;

  float abilityTimer = 0.0f;
  float ultimateTimer = 0.0f;
  float beforeDashSpeed;

  float notHitCounter = 0.0f;

  bool shooting = false;
  unsigned int weapons[2];

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

  void shipUpdate(float dt);

  void useShipAbility();
  void useShipUltimate();

  void normalShipUpdate(float dt);
  void normalShipAbility();
  void normalShipUltimate();
};

#endif
