#define GLM_ENABLE_EXPERIMENTAL
#include "../include/player.h"
#include "../include/bullet.h"
#include "../include/glm/gtx/quaternion.hpp"
#include "../include/glm/gtx/string_cast.hpp"
#include "../include/key.h"
#include "../include/ships.h"

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

  updateCamera();
}

glm::mat4 Player::getViewMatrix(glm::vec3 bossPosition) {
  glm::mat4 view = glm::mat4_cast(glm::conjugate(orientation));
  // model = glm::rotate(model, timePassed * 3.0f,
  //                     glm::normalize(glm::vec3(2.0f, 0.0f, 1.0)));

  glm::vec3 cameraUp = glm::normalize(orientation * global::cameraUp);
  switch (viewDirection) {
  case 1:
    view = glm::translate(view, -position);
    break;
  case 2:
    view = glm::rotate(view, float(M_PI), cameraUp);
    view = glm::translate(view, -position);
    break;
  case 3:
    view = glm::lookAt(position, bossPosition, cameraUp);
    break;
  case 4:
    glm::vec3 cameraFront = glm::normalize(orientation * global::cameraFront);
    glm::vec3 thirdPerson = -position + cameraFront * 10.0f - cameraUp * 2.0f;
    view = glm::translate(view, thirdPerson);
    break;
  }

  return view;
}

void Player::handleKeyboardInput(GLFWwindow *window, float dt) {

  float cameraSpeed = ship::shipTurnSpeed[ship] * dt;
  float xOffset = 0.0f;
  float yOffset = 0.0f;
  float zOffset = 0.0f;

  if (glfwGetKey(window, keys::gameplay.forward) == GLFW_PRESS) {
    speed += addSpeed(speed, ship::shipMaxSpeed[ship],
                      ship::shipAcceleration[ship], dt);
  }

  if (glfwGetKey(window, keys::gameplay.backward) == GLFW_PRESS) {
    speed += subtractSpeed(speed, ship::shipBreakStrength[ship], dt);
    if (speed < 0.0f)
      speed = 0.0f;
  }

  if (glfwGetKey(window, keys::gameplay.pitchUp) == GLFW_PRESS) {
    yOffset += cameraSpeed;
  }
  if (glfwGetKey(window, keys::gameplay.pitchDown) == GLFW_PRESS) {
    yOffset -= cameraSpeed;
  }

  if (glfwGetKey(window, keys::gameplay.rollRight) == GLFW_PRESS) {
    zOffset += cameraSpeed * 1.5f;
  }
  if (glfwGetKey(window, keys::gameplay.rollLeft) == GLFW_PRESS) {
    zOffset -= cameraSpeed * 1.53;
  }

  if (glfwGetKey(window, keys::gameplay.yawLeft) == GLFW_PRESS) {
    xOffset -= cameraSpeed * 0.5f;
  }
  if (glfwGetKey(window, keys::gameplay.yawRight) == GLFW_PRESS) {
    xOffset += cameraSpeed * 0.5f;
  }

  if (glfwGetKey(window, keys::gameplay.shoot) == GLFW_PRESS) {
    shootBullet();

    if (weapons[weaponIndex] == player::chargeRifle) {
      if (shootCounter < global::maxCounter)
        shootCounter += dt * shootSpeedBoost;
    }
  } else {
    if (weapons[weaponIndex] == player::chargeRifle)
      shootChargeRifle();

    laser->on = false;
  }

  if (weapons[weaponIndex] != player::laserCannon) {
    laser->on = false;
  }

  if (glfwGetKey(window, keys::camera.back) == GLFW_PRESS) {
    viewDirection = 2;
  } else if (glfwGetKey(window, keys::camera.boss) == GLFW_PRESS) {
    viewDirection = 3;
  } else if (glfwGetKey(window, keys::camera.thirdPerson) == GLFW_PRESS) {
    viewDirection = 4;
  } else {
    viewDirection = 1;
  }

  if (glfwGetKey(window, keys::gameplay.switchWeapon) == GLFW_PRESS &&
      canWeaponSwap) {
    weaponIndex = (weaponIndex + 1) % 2;
    canWeaponSwap = false;
    shootCounter = 0.0f;
    blade->spinCounter = 0.0f;
  }
  if (glfwGetKey(window, keys::gameplay.switchWeapon) != GLFW_PRESS) {
    canWeaponSwap = true;
  }

  if (glfwGetKey(window, keys::gameplay.ability) == GLFW_PRESS) {
    useShipAbility();
  }

  if (glfwGetKey(window, keys::gameplay.ultimate) == GLFW_PRESS) {
    useShipUltimate();
  }

  float yawAngle = -xOffset * cameraSpeed;
  float pitchAngle = -yOffset * cameraSpeed * 0.5;
  float rollAngle = zOffset * cameraSpeed;

  glm::vec3 cameraFront = glm::normalize(orientation * global::cameraFront);
  glm::vec3 cameraRight = glm::normalize(orientation * global::cameraRight);
  glm::vec3 cameraUp = glm::normalize(orientation * global::cameraUp);

  glm::quat qYaw = glm::angleAxis(glm::radians(yawAngle), cameraUp);
  glm::quat qPitch = glm::angleAxis(glm::radians(pitchAngle), cameraRight);
  glm::quat qRoll = glm::angleAxis(glm::radians(rollAngle), cameraFront);

  orientation = glm::normalize(qRoll * qYaw * qPitch * orientation);

  updateCamera();
}

void Player::update(float dt) {
  updateCameraMovement(dt);
  if (weapons[weaponIndex] != player::chargeRifle &&
      shootCounter < global::maxCounter)
    shootCounter += dt * shootSpeedBoost;

  shipUpdate(dt);

  blade->update(dt, position, orientation);

  if (weapons[weaponIndex] == player::laserCannon) {
    float downOffset = 1.0f;
    float dPitch = -std::asin(downOffset / bullet::laser.length);

    // float dPitch = glm::radians(-9.2069);

    glm::quat laserOrientation =
        glm::rotate(orientation, dPitch, glm::vec3(1.0f, 0.0f, 0.0f));

    // glm::vec3 laserFront = glm::normalize(orientation * CAMERA_FRONT);

    player::ShootArgs shootArgs = getShootArgs(-1.0f, 0.0f, 0.0f);
    // laser->update(dt,
    // shootArgs.bulletPosition + laserFront * LASER_LENGTH / 2.0f,
    // orientation, shootArgs.direction);
    laser->update(dt, shootArgs.bulletPosition, laserOrientation);
  }

  if (weapons[weaponIndex] == player::swingBlade) {
    if (blade->spinCounter >= 0.0f) {
      blade->spinCounter -= dt * shootSpeedBoost;
    }
  }
}

void Player::updateCamera() {
  front = glm::normalize(orientation * global::cameraFront);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

void Player::updateCameraMovement(float dt) {
  front = glm::normalize(orientation * global::cameraFront);

  speed += applyDrag(speed, ship::shipDragStrength[ship], dt);

  fov = 45.0f + (speed / ship::shipMaxSpeed[ship]) * 20.0f;

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
  case player::machineGun:
    shootMachineGun();
    break;
  case player::shotGun:
    shootShotGun();
    break;
  case player::homingMissile:
    shootHomingMissile();
    break;
  case player::bombLauncher:
    shootBombLauncher();
    break;
  case player::zapRifle:
    shootZapRifle();
    break;
  case player::cannonBall:
    shootCannon();
    break;
  case player::laserCannon:
    shootLaser();
    break;
  case player::swingBlade:
    shootBlade();
    break;
  }
}

player::ShootArgs Player::getShootArgs(float yOffset, float xOffset,
                                       float bulletSpread) {
  player::ShootArgs shootArgs;

  glm::vec3 cameraUp = glm::normalize(orientation * global::cameraUp);
  glm::vec3 cameraRight = glm::normalize(orientation * global::cameraRight);

  glm::vec3 direction = glm::normalize(orientation * global::cameraFront);
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
  if (shootCounter < bullet::machineGun.cooldown)
    return;
  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(bullet::machineGun.bulletSize);
  glm::vec3 color = glm::vec3(1.0f);

  player::ShootArgs shootArgs =
      getShootArgs(-2.0f, 1.0f, bullet::machineGun.spread);

  Projectile projectile;
  projectile.bullet = std::make_unique<Bullet>(
      shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
      shootArgs.direction, orientation, scale, color, bullet::machineGun.speed,
      bullet::machineGun.damage);
  projectiles.push_back(std::move(projectile));
}

void Player::shootShotGun() {
  if (shootCounter < bullet::shotgun.cooldown)
    return;
  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(bullet::shotgun.bulletSize);
  glm::vec3 color = glm::vec3(1.0f);

  for (unsigned int i = 0; i < 25; i++) {
    player::ShootArgs shootArgs =
        getShootArgs(-2.0f, 1.0f, bullet::shotgun.spread);
    Projectile projectile;
    projectile.bullet = std::make_unique<Bullet>(
        shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
        shootArgs.direction, orientation, scale, color, bullet::shotgun.speed,
        bullet::shotgun.damage);
    projectiles.push_back(std::move(projectile));
  }
}

void Player::shootHomingMissile() {
  if (shootCounter < bullet::homingMissile.cooldown)
    return;
  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(bullet::homingMissile.bulletSize);
  glm::vec3 color = glm::vec3(1.0f);

  player::ShootArgs shootArgs =
      getShootArgs(-1.0f, 1.0f, bullet::homingMissile.spread);

  Projectile projectile;
  projectile.bullet = std::make_unique<HomingMissile>(
      shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
      shootArgs.direction, orientation, scale, color,
      bullet::homingMissile.speed, bullet::homingMissile.damage);
  projectiles.push_back(std::move(projectile));
}

void Player::shootBombLauncher() {
  if (shootCounter < bullet::bombLauncher.cooldown)
    return;

  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(bullet::bombLauncher.bulletSize);
  glm::vec3 color = glm::vec3(1.0f);

  player::ShootArgs shootArgs =
      getShootArgs(-2.0f, 1.0f, bullet::bombLauncher.spread);

  Projectile projectile;
  projectile.bullet = std::make_unique<BombBullet>(
      shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
      shootArgs.direction, orientation, scale, color,
      bullet::bombLauncher.speed, bullet::bombLauncher.damage,
      bullet::bombLauncher.explosionTimer);
  projectiles.push_back(std::move(projectile));
}

void Player::shootChargeRifle() {
  float holdAmount = 0.0f;
  bool full = false;

  if (shootCounter > bullet::chargeRifle.cooldown) {
    full = true;
  } else if (shootCounter > bullet::chargeRifle.cooldown / 4.0) {
  } else {
    return;
  }

  holdAmount =
      glm::clamp(shootCounter / bullet::chargeRifle.cooldown, 0.0f, 1.0f);
  float num = exp(bullet::chargeRifle.strength * holdAmount) - 1;
  float denom = exp(bullet::chargeRifle.strength) - 1;
  float charge = num / denom;

  shootCounter = 0.0f;

  glm::vec3 cameraUp = glm::normalize(orientation * global::cameraUp);
  glm::vec3 cameraRight = glm::normalize(orientation * global::cameraRight);

  glm::vec3 direction = glm::normalize(orientation * global::cameraFront);
  glm::vec3 bulletPosition = glm::vec3(0.0f);
  bulletPosition = position + cameraUp * -2.0f;

  glm::vec3 scale = glm::vec3(bullet::chargeRifle.bulletSize) * charge;
  if (full) {
    scale *= 2.0f;
  }
  glm::vec3 color = glm::vec3(1.0f);

  float speed =
      glm::clamp(bullet::chargeRifle.speed * charge,
                 bullet::chargeRifle.speed / 2.0f, bullet::chargeRifle.speed);
  float damage = bullet::chargeRifle.damage * charge;

  Projectile projectile;
  projectile.bullet =
      std::make_unique<Bullet>(bulletPosition, cameraRight, direction,
                               orientation, scale, color, speed, damage);
  projectiles.push_back(std::move(projectile));
}

void Player::shootZapRifle() {
  if (shootCounter < bullet::zapRifle.cooldown)
    return;
  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(bullet::zapRifle.bulletSize);
  glm::vec3 color = glm::vec3(1.0f);

  player::ShootArgs shootArgs =
      getShootArgs(0.0f, 1.0f, bullet::zapRifle.spread);

  Projectile projectile;
  projectile.bullet = std::make_unique<ZapBullet>(
      shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
      shootArgs.direction, orientation, scale, color, bullet::zapRifle.spread,
      bullet::zapRifle.damage);
  projectiles.push_back(std::move(projectile));
}

void Player::shootCannon() {
  if (shootCounter < bullet::cannon.cooldown)
    return;
  shootCounter = 0.0f;

  glm::vec3 scale = glm::vec3(bullet::cannon.bulletSize);
  glm::vec3 color = glm::vec3(1.0f);

  player::ShootArgs shootArgs =
      getShootArgs(-1.0f, 0.0f, bullet::cannon.spread);

  Projectile projectile;
  projectile.bullet = std::make_unique<Bullet>(
      shootArgs.bulletPosition, shootArgs.spin * shootArgs.direction,
      shootArgs.direction, orientation, scale, color, bullet::cannon.speed,
      bullet::cannon.damage);
  projectiles.push_back(std::move(projectile));
}

void Player::shootLaser() { laser->on = true; }

void Player::shootBlade() {
  if (blade->spinCounter < 0.0f) {
    blade->spinCounter = bullet::blade.spinTime;
  }
}

void Player::shipUpdate(float dt) {
  switch (ship) {
  case player::normalShip:
    normalShipUpdate(dt);
    break;
  case player::tankShip:
    break;
  case player::timeShip:
    break;
  case player::vampireShip:
    break;
  case player::speedShip:
    break;
  case player::parryShip:
    break;
  }
}

void Player::useShipAbility() {
  switch (ship) {
  case player::normalShip:
    normalShipAbility();
    break;
  case player::tankShip:
    break;
  case player::timeShip:
    break;
  case player::vampireShip:
    break;
  case player::speedShip:
    break;
  case player::parryShip:
    break;
  }
}

void Player::useShipUltimate() {
  switch (ship) {
  case player::normalShip:
    normalShipUltimate();
    break;
  case player::tankShip:
    break;
  case player::timeShip:
    break;
  case player::vampireShip:
    break;
  case player::speedShip:
    break;
  case player::parryShip:
    break;
  }
}

void Player::normalShipUpdate(float dt) {
  if (abilityCounter < global::maxCounter) {
    abilityCounter += dt;
  }
  if (ultimateCounter < global::maxCounter) {
    ultimateCounter += dt;
  }

  if (abilityTimer > 0.0f) {
    float percent = (ship::normalShip.abilityLength - abilityTimer) /
                    ship::normalShip.abilityLength;
    float y = exp(-ship::normalShip.abilityStrength * percent);

    speed = ship::normalShip.abilitySpeed * y + beforeDashSpeed;
    abilityTimer -= dt;
  }

  if (ultimateTimer > 0.0f) {
    ultimateTimer -= dt;
  }

  if (notHitCounter < global::maxCounter) {
    notHitCounter += dt;
  }

  float x = notHitCounter / ship::normalShip.maxPassiveTime;

  float num = 1 - exp(-ship::normalShip.passiveStrength * x);
  float denom = 1 - exp(-ship::normalShip.passiveStrength);
  float y = (ship::normalShip.maxPassiveBoost - 1.0f) * (num / denom) + 1.0f;

  damageBoost = glm::clamp(y, 1.0f, ship::normalShip.maxPassiveBoost);
  shootSpeedBoost = glm::clamp(y, 1.0f, ship::normalShip.maxPassiveBoost);
  if (ultimateTimer > 0.0f) {
    shootSpeedBoost *= ship::normalShip.ultimateBoost;
  }
}
void Player::normalShipAbility() {
  if (abilityCounter < ship::normalShip.abilityCooldown)
    return;

  beforeDashSpeed = speed;
  speed = ship::normalShip.abilitySpeed;
  abilityCounter = 0;
  abilityTimer = ship::normalShip.abilityLength;
}

void Player::normalShipUltimate() {
  if (ultimateCounter < ship::normalShip.ultimateCooldown)
    return;

  ultimateCounter = 0;
  ultimateTimer = ship::normalShip.ultimateLength;
}
