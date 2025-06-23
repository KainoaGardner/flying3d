#ifndef PLAYER_H
#define PLAYER_H

#include "../include/bullet.h"
#include "../include/config.h"
#include "../include/geomety.h"
#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtx/quaternion.hpp"
#include "../include/ships.h"
#include <GLFW/glfw3.h>
#include <string>

namespace player {
enum Weapons {
  machineGun,
  shotGun,
  homingMissile,
  bombLauncher,
  chargeRifle,
  zapRifle,
  cannonBall,
  laserCannon,
  swingBlade,
};

enum Ships {
  normalShip,
  tankShip,
  timeShip,
  speedShip,
  parryShip,
  vampireShip,
};

struct ShootArgs {
  glm::vec3 bulletPosition;
  glm::vec3 direction;
  float spin;
};

struct DisplayContext {
  glm::mat4 projection;
  glm::mat4 view;
  glm::vec3 bossPos;
  glm::mat4 textProjection;
};

struct PlayerStruct {
  const float respawnTime = 100.0f;
};

extern PlayerStruct playerStruct;

} // namespace player

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

  float damageBoost = 1.0f;
  float speedBoost = 1.0f;
  float shootSpeedBoost = 1.0f;
  float damageReductionAmount = 1.0f;

  float timeSlowAmount = 1.0f;

  float health;

  Laser *laser = new Laser(glm::vec3(1.0f), bullet::laser.damage);
  Blade *blade = new Blade(glm::vec3(bullet::blade.size), glm::vec3(1.0f),
                           bullet::blade.damage);

  glm::quat orientation;
  unsigned int viewDirection = 1;

  Player(glm::vec3 positionIn, glm::vec3 worldUpIn, glm::vec3 frontIn,
         glm::quat orientaionIn, unsigned int shipIn, unsigned int weapons[2]);

  glm::mat4 getViewMatrix(glm::vec3 bossPosition);

  void handleKeyboardInput();
  void handleMouseInput(float xOffset, float yOffset, bool constrain = true);
  void handleScrollInput(float yOffset, float minFov, float maxFov);

  void update();

  void takeDamage(float damage);
  void healShip(float addHealth);

  float shootCounter = 0.0f;
  float abilityCounter = global::maxCounter;
  float ultimateCounter = global::maxCounter;
  // float ultimateCounter = 0.0f;
  float abilityTimer = 0.0f;
  float ultimateTimer = 0.0f;
  float deathCounter = 0.0f;

  float getBulletTimeSlow(glm::vec3 bulletPosition);

  void displayScreen(player::DisplayContext);

  bool alive = true;

private:
  float maxSpeed;

  bool leftGun = false;
  bool canWeaponSwap = true;

  float beforeDashSpeed;

  float notHitCounter = 0.0f;

  bool shooting = false;
  unsigned int weapons[2];

  void updateCamera();
  void updateCameraMovement();

  float addSpeed(float currentSpeed, float maxSpeed, float acceleration);
  float subtractSpeed(float currentSpeed, float brakeStrength);
  float applyDrag(float currentSpeed, float dragRate);

  player::ShootArgs getShootArgs(float yOffset, float xOffset,
                                 float bulletSpread);

  void displayHealth(player::DisplayContext displayContext);
  void displayArrow(player::DisplayContext displayContext);
  void displayReload(player::DisplayContext displayContext);
  void displayCooldown(player::DisplayContext displayContext);
  void displayCooldownText(player::DisplayContext displayContext);

  bool checkBulletCollision();

  void shootBullet();
  void shootMachineGun();
  void shootShotGun();
  void shootHomingMissile();
  void shootBombLauncher();
  void shootChargeRifle();
  void shootZapRifle();
  void shootCannon();
  void shootLaser();
  void shootBlade();

  void shipUpdate();

  void useShipAbility();
  void useShipUltimate();

  void normalShipUpdate();
  void normalShipAbility();
  void normalShipUltimate();

  void tankShipUpdate();
  void tankShipAbility();
  void tankShipUltimate();

  void timeShipUpdate();
  void timeShipInput();
  void timeShipAbility();
  void timeShipUltimate();

  void speedShipUpdate();
  void speedShipInput();
  void speedShipAbility();
  void speedShipUltimate();

  void parryShipAbility();
  void parryShipUltimate();

  void vampireShipUpdate();
  void vampireShipAbility();
  void vampireShipUltimate();
};

#endif
