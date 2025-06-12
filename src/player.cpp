#define GLM_ENABLE_EXPERIMENTAL
#include "../include/player.h"
#include "../include/bullet.h"
#include "../include/glm/gtx/quaternion.hpp"
#include "../include/glm/gtx/string_cast.hpp"
#include "../include/key.h"
#include "../include/ships.h"

const float MAX_COUNTER = std::numeric_limits<float>::max();

const glm::vec3 CAMERA_POSITION = glm::vec3(0.0f, 0.0f, 3.0f);
const glm::vec3 CAMERA_FRONT = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 CAMERA_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 CAMERA_RIGHT = glm::vec3(1.0f, 0.0f, 0.0f);

const float FOV = 45.0f;

const glm::quat CAMERA_ORIENTATION = glm::quat(1, 0, 0, 0);

Player::Player(glm::vec3 positionIn, glm::vec3 worldUpIn, glm::vec3 frontIn,
               glm::quat orientationIn, unsigned int shipIn,
               unsigned int weaponsIn[2]) {
  position = positionIn;
  worldUp = worldUpIn;
  front = frontIn;
  orientation = orientationIn;
  weapons[0] = weaponsIn[0];
  weapons[1] = weaponsIn[1];
  ship = shipIn;

  viewDirection = 1;

  updateCamera();
}

glm::mat4 Player::getViewMatrix(glm::vec3 bossPosition) {
  glm::mat4 view = glm::mat4_cast(glm::conjugate(orientation));
  // model = glm::rotate(model, timePassed * 3.0f,
  //                     glm::normalize(glm::vec3(2.0f, 0.0f, 1.0)));

  glm::vec3 cameraUp = glm::normalize(orientation * CAMERA_UP);
  if (viewDirection == 2) {
    view = glm::rotate(view, float(M_PI), cameraUp);
  }
  if (viewDirection == 3) {
    view = glm::lookAt(position, bossPosition, cameraUp);
  }

  if (viewDirection != 3)
    view = glm::translate(view, -position);

  return view;
}

void Player::handleKeyboardInput(GLFWwindow *window, float dt) {

  float cameraSpeed = shipTurnSpeed[ship] * dt;
  float xOffset = 0.0f;
  float yOffset = 0.0f;
  float zOffset = 0.0f;

  if (glfwGetKey(window, FORWARD_KEY) == GLFW_PRESS) {
    speed += addSpeed(speed, shipMaxSpeed[ship], shipAcceleration[ship], dt);
  }

  if (glfwGetKey(window, BACKWARD_KEY) == GLFW_PRESS) {
    speed += subtractSpeed(speed, shipBreakStrength[ship], dt);
    if (speed < 0.0f)
      speed = 0.0f;
  }

  if (glfwGetKey(window, PITCH_UP_KEY) == GLFW_PRESS) {
    yOffset += cameraSpeed;
  }
  if (glfwGetKey(window, PITCH_DOWN_KEY) == GLFW_PRESS) {
    yOffset -= cameraSpeed;
  }

  if (glfwGetKey(window, ROLL_RIGHT_KEY) == GLFW_PRESS) {
    zOffset += cameraSpeed * 1.5f;
  }
  if (glfwGetKey(window, ROLL_LEFT_KEY) == GLFW_PRESS) {
    zOffset -= cameraSpeed * 1.53;
  }

  if (glfwGetKey(window, YAW_LEFT_KEY) == GLFW_PRESS) {
    xOffset -= cameraSpeed * 0.5f;
  }
  if (glfwGetKey(window, YAW_RIGHT_KEY) == GLFW_PRESS) {
    xOffset += cameraSpeed * 0.5f;
  }

  if (glfwGetKey(window, SHOOT_KEY) == GLFW_PRESS) {
    shootBullet();

    if (weapons[weaponIndex] == chargeRifle) {
      if (shootCounter < MAX_COUNTER)
        shootCounter += dt * shootSpeedBoost;
    }
  } else {
    if (weapons[weaponIndex] == chargeRifle)
      shootChargeRifle();
  }

  if (glfwGetKey(window, CAMERA_BACK_KEY) == GLFW_PRESS) {
    viewDirection = 2;
  } else if (glfwGetKey(window, CAMERA_BOSS_KEY) == GLFW_PRESS) {
    viewDirection = 3;
  } else {
    viewDirection = 1;
  }

  if (glfwGetKey(window, SWITCH_WEAPON) == GLFW_PRESS && canWeaponSwap) {
    weaponIndex = (weaponIndex + 1) % 2;
    canWeaponSwap = false;
    shootCounter = 0.0f;
  }
  if (glfwGetKey(window, SWITCH_WEAPON) != GLFW_PRESS) {
    canWeaponSwap = true;
  }

  if (glfwGetKey(window, ABILITY_KEY) == GLFW_PRESS) {
    useShipAbility();
  }

  if (glfwGetKey(window, ULTIMATE_KEY) == GLFW_PRESS) {
    useShipUltimate();
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

void Player::update(float dt) {
  updateCameraMovement(dt);
  if (weapons[weaponIndex] != chargeRifle && shootCounter < MAX_COUNTER)
    shootCounter += dt * shootSpeedBoost;

  shipUpdate(dt);
}

void Player::updateCamera() {
  front = glm::normalize(orientation * CAMERA_FRONT);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

void Player::updateCameraMovement(float dt) {
  front = glm::normalize(orientation * CAMERA_FRONT);

  speed += applyDrag(speed, shipDragStrength[ship], dt);

  fov = 45.0f + (speed / shipMaxSpeed[ship]) * 20.0f;

  float vel = speed;

  position += front * dt * vel;
}

float Player::addSpeed(float currentSpeed, float maxSpeed, float acceleration,
                       float dt) {
  float dSpeed = maxSpeed - currentSpeed;
  return acceleration * dSpeed * dt;
}

float Player::subtractSpeed(float currentSpeed, float brakeStrength, float dt) {
  return -brakeStrength * currentSpeed * dt;
}

float Player::applyDrag(float currentSpeed, float dragRate, float dt) {
  return -dragRate * currentSpeed * dt;
}

void Player::shootBullet() {
  switch (weapons[weaponIndex]) {
  case machineGun:
    shootMachineGun();
    break;
  case shotGun:
    shootShotGun();
    break;
  case homingMissile:
    shootHomingMissile();
    break;
  case bombLauncher:
    shootBombLauncher();
    break;
  case zapRifle:
    shootZapRifle();
    break;
  case cannon:
    shootCannon();
    break;
  case laser:
    shootLaser();
    break;
  case blade:
    shootBlade();
    break;
  }
}

ShootArgs Player::getShootArgs(float yOffset, float xOffset,
                               float bulletSpread) {
  ShootArgs shootArgs;

  glm::vec3 cameraUp = glm::normalize(orientation * CAMERA_UP);
  glm::vec3 cameraRight = glm::normalize(orientation * CAMERA_RIGHT);

  glm::vec3 direction = glm::normalize(orientation * CAMERA_FRONT);
  direction.x += ((float(rand() % 100) / 100.0) - 0.5) / bulletSpread;
  direction.y += ((float(rand() % 100) / 100.0) - 0.5) / bulletSpread;
  direction.z += ((float(rand() % 100) / 100.0) - 0.5) / bulletSpread;

  glm::vec3 bulletPosition = glm::vec3(0.0f);
  float spin = -1.0f;

  if (leftGun) {
    bulletPosition = position + cameraUp * yOffset + cameraRight * -xOffset;
    spin = 1.0;
    leftGun = false;

  } else {
    bulletPosition = position + cameraUp * yOffset + cameraRight * xOffset;
    leftGun = true;
  }

  shootArgs.bulletPosition = bulletPosition;
  shootArgs.direction = direction;
  shootArgs.spin = spin;

  return shootArgs;
}

void Player::shootMachineGun() {
  if (shootCounter < MACHINE_GUN_COOLDOWN)
    return;
  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(MACHINE_GUN_BULLET_SIZE);
  glm::vec3 color = glm::vec3(1.0f);

  ShootArgs shootArgs = getShootArgs(-2.0f, 1.0f, MACHINE_GUN_SPREAD);

  Projectile projectile;
  projectile.bullet = std::make_unique<Bullet>(
      shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
      shootArgs.direction, orientation, scale, color, MACHINE_GUN_SPEED,
      MACHINE_GUN_DAMAGE);
  projectiles.push_back(std::move(projectile));
}

void Player::shootShotGun() {
  if (shootCounter < SHOTGUN_COOLDOWN)
    return;
  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(SHOTGUN_BULLET_SIZE);
  glm::vec3 color = glm::vec3(1.0f);

  for (unsigned int i = 0; i < 25; i++) {
    ShootArgs shootArgs = getShootArgs(-2.0f, 1.0f, SHOTGUN_SPREAD);
    Projectile projectile;
    projectile.bullet = std::make_unique<Bullet>(
        shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
        shootArgs.direction, orientation, scale, color, SHOTGUN_SPEED,
        SHOTGUN_DAMAGE);
    projectiles.push_back(std::move(projectile));
  }
}

void Player::shootHomingMissile() {
  if (shootCounter < HOMING_MISSILE_COOLDOWN)
    return;
  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(HOMING_MISSILE_BULLET_SIZE);
  glm::vec3 color = glm::vec3(1.0f);

  ShootArgs shootArgs = getShootArgs(-1.0f, 1.0f, HOMING_MISSILE_SPREAD);

  Projectile projectile;
  projectile.homingMissile = std::make_unique<HomingMissile>(
      shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
      shootArgs.direction, orientation, scale, color, HOMING_MISSILE_SPEED,
      HOMING_MISSILE_DAMAGE);
  projectiles.push_back(std::move(projectile));
}

void Player::shootBombLauncher() {
  if (shootCounter < BOMB_LAUNCHER_COOLDOWN)
    return;

  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(BOMB_LAUNCHER_BULLET_SIZE);
  glm::vec3 color = glm::vec3(1.0f);

  ShootArgs shootArgs = getShootArgs(-2.0f, 1.0f, BOMB_LAUNCHER_SPREAD);

  Projectile projectile;
  projectile.bombBullet = std::make_unique<BombBullet>(
      shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
      shootArgs.direction, orientation, scale, color, BOMB_LAUNCHER_SPEED,
      BOMB_LAUNCHER_DAMAGE, BOMB_LAUNCHER_EXPLOSION_TIMER);
  projectiles.push_back(std::move(projectile));
}

void Player::shootChargeRifle() {
  float holdAmount = 0.0f;
  float k = 4.0f;
  bool full = false;

  if (shootCounter > CHARGE_RIFLE_COOLDOWN) {
    full = true;
  } else if (shootCounter > CHARGE_RIFLE_COOLDOWN / 4.0) {
  } else {
    return;
  }

  holdAmount = glm::clamp(shootCounter / CHARGE_RIFLE_COOLDOWN, 0.0f, 1.0f);
  float num = exp(k * holdAmount) - 1;
  float denom = exp(k) - 1;
  float charge = num / denom;

  shootCounter = 0.0f;

  glm::vec3 cameraUp = glm::normalize(orientation * CAMERA_UP);
  glm::vec3 cameraRight = glm::normalize(orientation * CAMERA_RIGHT);

  glm::vec3 direction = glm::normalize(orientation * CAMERA_FRONT);
  glm::vec3 bulletPosition = glm::vec3(0.0f);
  bulletPosition = position + cameraUp * -2.0f;

  glm::vec3 scale = glm::vec3(CHARGE_RIFLE_BULLET_SIZE) * charge;
  if (full) {
    scale *= 2.0f;
  }
  glm::vec3 color = glm::vec3(1.0f);

  float speed = glm::clamp(CHARGE_RIFLE_SPEED * charge,
                           CHARGE_RIFLE_SPEED / 2.0f, CHARGE_RIFLE_SPEED);
  float damage = CHARGE_RIFLE_DAMAGE * charge;

  Projectile projectile;
  projectile.bullet =
      std::make_unique<Bullet>(bulletPosition, cameraRight, direction,
                               orientation, scale, color, speed, damage);
  projectiles.push_back(std::move(projectile));
}

void Player::shootZapRifle() {
  if (shootCounter < ZAP_RIFLE_COOLDOWN)
    return;
  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(ZAP_RIFLE_BULLET_SIZE);
  glm::vec3 color = glm::vec3(1.0f);

  ShootArgs shootArgs = getShootArgs(0.0f, 1.0f, ZAP_RIFLE_SPREAD);

  Projectile projectile;
  projectile.bullet = std::make_unique<Bullet>(
      shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
      shootArgs.direction, orientation, scale, color, ZAP_RIFLE_SPEED,
      ZAP_RIFLE_DAMAGE);
  projectiles.push_back(std::move(projectile));
}

void Player::shootCannon() {
  // if (shootCounter < ZAP_RIFLE_COOLDOWN)
  //   return;
  // shootCounter = 0.0f;
  //
  // glm::vec3 scale = glm::vec3(ZAP_RIFLE_BULLET_SIZE);
  // glm::vec3 color = glm::vec3(1.0f);
  //
  // ShootArgs shootArgs = getShootArgs(ZAP_RIFLE_SPREAD);
  //
  // Projectile projectile;
  // projectile.bullet = std::make_unique<Bullet>(
  //     shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
  //     shootArgs.direction, orientation, scale, color, ZAP_RIFLE_SPEED,
  //     ZAP_RIFLE_DAMAGE);
  // projectiles.push_back(std::move(projectile));
}

void Player::shootLaser() {
  // if (shootCounter < ZAP_RIFLE_COOLDOWN)
  //   return;
  // shootCounter = 0.0f;
  //
  // glm::vec3 scale = glm::vec3(ZAP_RIFLE_BULLET_SIZE);
  // glm::vec3 color = glm::vec3(1.0f);
  //
  // ShootArgs shootArgs = getShootArgs(ZAP_RIFLE_SPREAD);
  //
  // Projectile projectile;
  // projectile.bullet = std::make_unique<Bullet>(
  //     shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
  //     shootArgs.direction, orientation, scale, color, ZAP_RIFLE_SPEED,
  //     ZAP_RIFLE_DAMAGE);
  // projectiles.push_back(std::move(projectile));
}

void Player::shootBlade() {
  // if (shootCounter < ZAP_RIFLE_COOLDOWN)
  //   return;
  // shootCounter = 0.0f;
  //
  // glm::vec3 scale = glm::vec3(ZAP_RIFLE_BULLET_SIZE);
  // glm::vec3 color = glm::vec3(1.0f);
  //
  // ShootArgs shootArgs = getShootArgs(ZAP_RIFLE_SPREAD);
  //
  // Projectile projectile;
  // projectile.bullet = std::make_unique<Bullet>(
  //     shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
  //     shootArgs.direction, orientation, scale, color, ZAP_RIFLE_SPEED,
  //     ZAP_RIFLE_DAMAGE);
  // projectiles.push_back(std::move(projectile));
}

void Player::shipUpdate(float dt) {
  switch (ship) {
  case normalShip:
    normalShipUpdate(dt);
    break;
  case tankShip:
    break;
  case timeShip:
    break;
  case vampireShip:
    break;
  case speedShip:
    break;
  case parryShip:
    break;
  }
}

void Player::useShipAbility() {
  switch (ship) {
  case normalShip:
    normalShipAbility();
    break;
  case tankShip:
    break;
  case timeShip:
    break;
  case vampireShip:
    break;
  case speedShip:
    break;
  case parryShip:
    break;
  }
}

void Player::useShipUltimate() {
  switch (ship) {
  case normalShip:
    normalShipUltimate();
    break;
  case tankShip:
    break;
  case timeShip:
    break;
  case vampireShip:
    break;
  case speedShip:
    break;
  case parryShip:
    break;
  }
}

void Player::normalShipUpdate(float dt) {
  if (abilityCounter < MAX_COUNTER) {
    abilityCounter += dt;
  }
  if (ultimateCounter < MAX_COUNTER) {
    ultimateCounter += dt;
  }

  if (abilityTimer > 0.0f) {
    float percent = (NORMAL_SHIP_ABILITY_LENGTH - abilityTimer) /
                    NORMAL_SHIP_ABILITY_LENGTH;
    float y = exp(-NORMAL_SHIP_ABILITY_STRENGTH * percent);

    speed = NORMAL_SHIP_ABILITY_SPEED * y + beforeDashSpeed;
    abilityTimer -= dt;
  }

  if (ultimateTimer > 0.0f) {
    ultimateTimer -= dt;
  }

  if (notHitCounter < MAX_COUNTER) {
    notHitCounter += dt;
  }

  float x = notHitCounter / NORMAL_SHIP_MAX_PASSIVE_TIME;

  float num = 1 - exp(-NORMAL_SHIP_PASSIVE_STRENGTH * x);
  float denom = 1 - exp(-NORMAL_SHIP_PASSIVE_STRENGTH);
  float y = (NORMAL_SHIP_MAX_PASSIVE_BOOST - 1.0f) * (num / denom) + 1.0f;

  damageBoost = glm::clamp(y, 1.0f, NORMAL_SHIP_MAX_PASSIVE_BOOST);
  shootSpeedBoost = glm::clamp(y, 1.0f, NORMAL_SHIP_MAX_PASSIVE_BOOST);
  if (ultimateTimer > 0.0f) {
    shootSpeedBoost *= NORMAL_SHIP_ULTIMATE_BOOST;
  }
}
void Player::normalShipAbility() {
  if (abilityCounter < NORMAL_SHIP_ABILITY_COOLDOWN)
    return;

  beforeDashSpeed = speed;
  speed = NORMAL_SHIP_ABILITY_SPEED;
  abilityCounter = 0;
  abilityTimer = NORMAL_SHIP_ABILITY_LENGTH;
}

void Player::normalShipUltimate() {
  if (ultimateCounter < NORMAL_SHIP_ULTIMATE_COOLDOWN)
    return;

  ultimateCounter = 0;
  ultimateTimer = NORMAL_SHIP_ULTIMATE_LENGTH;
}
