#define GLM_ENABLE_EXPERIMENTAL
#include "../include/player.h"
#include "../include/display.h"
#include "../include/glm/gtc/quaternion.hpp"
#include "../include/key.h"
#include <iostream>

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

  health = ship::shipMaxHealth[ship];
  maxSpeed = ship::shipMaxSpeed[ship];

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

void Player::handleKeyboardInput() {
  float cameraSpeed = ship::shipTurnSpeed[ship];
  float xOffset = 0.0f;
  float yOffset = 0.0f;
  float zOffset = 0.0f;

  if (keys::actionPressed[keys::forward]) {
    speed +=
        addSpeed(speed, maxSpeed, ship::shipAcceleration[ship]) * speedBoost;
  }

  if (keys::actionPressed[keys::backward]) {
    speed += subtractSpeed(speed, ship::shipBreakStrength[ship]) * speedBoost;
    if (speed < 0.0f)
      speed = 0.0f;
  }

  if (keys::actionPressed[keys::pitchUp]) {
    yOffset += cameraSpeed;
  }
  if (keys::actionPressed[keys::pitchDown]) {
    yOffset -= cameraSpeed;
  }

  if (keys::actionPressed[keys::rollRight]) {
    zOffset += cameraSpeed * 1.5f;
  }
  if (keys::actionPressed[keys::rollLeft]) {
    zOffset -= cameraSpeed * 1.5f;
  }

  if (keys::actionPressed[keys::yawLeft]) {
    xOffset -= cameraSpeed * 0.5f;
  }
  if (keys::actionPressed[keys::yawRight]) {
    xOffset += cameraSpeed * 0.5f;
  }

  if (keys::actionPressed[keys::shoot]) {
    shootBullet();

    if (weapons[weaponIndex] == player::chargeRifle) {
      if (shootCounter < global::maxCounter)
        shootCounter += 1.0f * shootSpeedBoost;
    }
  } else {
    if (weapons[weaponIndex] == player::chargeRifle)
      shootChargeRifle();

    laser->on = false;
  }

  if (weapons[weaponIndex] != player::laserCannon) {
    laser->on = false;
  }

  if (keys::actionPressed[keys::backCamera]) {
    viewDirection = 2;
  } else if (keys::actionPressed[keys::bossCamera]) {
    viewDirection = 3;
  } else if (keys::actionPressed[keys::thirdPersonCamera]) {
    viewDirection = 4;
  } else {
    viewDirection = 1;
  }

  if (keys::actionPressed[keys::switchWeapon] && canWeaponSwap) {
    weaponIndex = (weaponIndex + 1) % 2;
    keys::actionPressed[keys::switchWeapon] = false;
    // canWeaponSwap = false;
    shootCounter = 0.0f;
    blade->spinCounter = 0.0f;
  }
  // if (keys::actionPressed[keys::switchWeapon]) {
  //   canWeaponSwap = true;
  // }

  if (keys::actionPressed[keys::ability]) {
    useShipAbility();
  }

  if (keys::actionPressed[keys::ultimate]) {
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

  if (ship == player::timeShip) {
    timeShipInput();
  }
  if (ship == player::speedShip) {
    speedShipInput();
  }

  updateCamera();
}

void Player::update() {
  updateCameraMovement();
  if (weapons[weaponIndex] != player::chargeRifle &&
      shootCounter < global::maxCounter)
    shootCounter += 1.0f * shootSpeedBoost;

  shipUpdate();

  blade->update(position, orientation);

  if (!(ship == player::tankShip && abilityTimer > 0.0f)) {

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
      laser->update(shootArgs.bulletPosition, laserOrientation);
    }

    if (weapons[weaponIndex] == player::swingBlade) {
      if (blade->spinCounter >= 0.0f) {
        blade->spinCounter -= 1.0f * shootSpeedBoost;
      }
    }
  }
}

void Player::updateCamera() {
  front = glm::normalize(orientation * global::cameraFront);
  right = glm::normalize(glm::cross(front, worldUp));
  up = glm::normalize(glm::cross(right, front));
}

void Player::updateCameraMovement() {
  front = glm::normalize(orientation * global::cameraFront);

  speed += applyDrag(speed, ship::shipDragStrength[ship]);

  fov = 45.0f + (speed / ship::shipMaxSpeed[ship]) * 20.0f;

  float vel = speed;

  position += front * vel;
}

float Player::addSpeed(float currentSpeed, float maxSpeed, float acceleration) {
  float dSpeed = maxSpeed - currentSpeed;
  return acceleration * dSpeed;
}

float Player::subtractSpeed(float currentSpeed, float brakeStrength) {
  return -brakeStrength * currentSpeed;
}

float Player::applyDrag(float currentSpeed, float dragRate) {
  return -dragRate * currentSpeed;
}

void Player::shootBullet() {
  if (ship == player::tankShip && abilityTimer > 0.0f) {
    return;
  }

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
      shootArgs.direction, orientation, scale, color,
      bullet::machineGun.speed + speed, bullet::machineGun.damage * damageBoost,
      false);
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
        shootArgs.direction, orientation, scale, color,
        bullet::shotgun.speed + speed, bullet::shotgun.damage * damageBoost,
        false);
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
      bullet::homingMissile.speed + speed,
      bullet::homingMissile.damage * damageBoost, false);
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
      bullet::bombLauncher.speed + speed,
      bullet::bombLauncher.damage * damageBoost, false,
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
  float damage = bullet::chargeRifle.damage * charge * damageBoost;

  Projectile projectile;
  projectile.bullet =
      std::make_unique<Bullet>(bulletPosition, cameraRight, direction,
                               orientation, scale, color, speed, damage, false);
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
      shootArgs.direction, orientation, scale, color,
      bullet::zapRifle.speed + speed, bullet::zapRifle.damage * damageBoost,
      false);
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
      shootArgs.direction, orientation, scale, color,
      bullet::cannon.speed + speed, bullet::cannon.damage * damageBoost, false);
  projectiles.push_back(std::move(projectile));
}

void Player::shootLaser() { laser->on = true; }

void Player::shootBlade() {
  if (blade->spinCounter < 0.0f) {
    blade->spinCounter = bullet::blade.spinTime;
  }
}

void Player::shipUpdate() {
  if (abilityCounter < global::maxCounter)
    abilityCounter += 1.0f;

  if (ultimateCounter < global::maxCounter)
    ultimateCounter += 1.0f;

  if (abilityTimer > 0.0f)
    abilityTimer -= 1.0f;

  if (ultimateTimer > 0.0f)
    ultimateTimer -= 1.0f;

  switch (ship) {
  case player::normalShip:
    normalShipUpdate();
    break;
  case player::tankShip:
    tankShipUpdate();
    break;
  case player::timeShip:
    timeShipUpdate();
    break;
  case player::speedShip:
    speedShipUpdate();
    break;
  case player::vampireShip:
    vampireShipUpdate();
    break;
  }
}

void Player::useShipAbility() {
  switch (ship) {
  case player::normalShip:
    normalShipAbility();
    break;
  case player::tankShip:
    tankShipAbility();
    break;
  case player::timeShip:
    timeShipAbility();
    break;
  case player::speedShip:
    speedShipAbility();
    break;
  case player::parryShip:
    parryShipAbility();
    break;
  case player::vampireShip:
    vampireShipAbility();
    break;
  }
}

void Player::useShipUltimate() {
  switch (ship) {
  case player::normalShip:
    normalShipUltimate();
    break;
  case player::tankShip:
    tankShipUltimate();
    break;
  case player::timeShip:
    timeShipUltimate();
    break;
  case player::speedShip:
    speedShipUltimate();
    break;
  case player::parryShip:
    parryShipUltimate();
    break;
  case player::vampireShip:
    vampireShipUltimate();
    break;
  }
}

void Player::normalShipUpdate() {
  if (abilityTimer > 0.0f) {
    float percent = (ship::normalShip.abilityLength - abilityTimer) /
                    ship::normalShip.abilityLength;
    float y = exp(-ship::normalShip.abilityStrength * percent);

    speed = ship::normalShip.abilitySpeed * y + beforeDashSpeed;
  }

  if (notHitCounter < global::maxCounter) {
    notHitCounter += 1.0f;
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

void Player::tankShipUpdate() {
  if (ultimateTimer > 0.0f) {
    damageReductionAmount = 0.0f;
  } else if (abilityTimer > 0.0f) {
    damageReductionAmount = 0.5f;
  } else {
    damageReductionAmount = 1.0f;
  }

  if (notHitCounter < global::maxCounter) {
    notHitCounter += 1.0f;
  }

  if (speed < ship::tankShip.passiveMinSpeed) {
    damageBoost = ship::tankShip.passiveDamageBoost;
  } else {
    damageBoost = 1.0f;
  }
}

void Player::tankShipAbility() {
  if (abilityCounter < ship::tankShip.abilityCooldown)
    return;

  abilityCounter = 0;
  abilityTimer = ship::tankShip.abilityLength;
}

void Player::tankShipUltimate() {
  if (ultimateCounter < ship::tankShip.ultimateCooldown)
    return;

  ultimateCounter = 0;
  ultimateTimer = ship::tankShip.ultimateLength;
}

void Player::timeShipUpdate() {
  if (ultimateTimer > 0.0f) {
    timeSlowAmount = 0.0f;
  } else {
    timeSlowAmount = 1.0f;
  }
}

void Player::timeShipAbility() {
  if (abilityCounter < ship::timeShip.abilityCooldown)
    return;

  abilityCounter = 0;
  abilityTimer = ship::timeShip.abilityLength;
}

void Player::timeShipUltimate() {
  if (ultimateCounter < ship::timeShip.ultimateCooldown)
    return;

  ultimateCounter = 0;
  ultimateTimer = ship::timeShip.ultimateLength;
}

float Player::getBulletTimeSlow(glm::vec3 bulletPosition) {
  float distance = glm::distance(bulletPosition, position);
  if (distance > ship::timeShip.passiveMaxDistance) {
    return 1.0f;
  }
  return ship::timeShip.passiveMaxTimeStop;
}

void Player::timeShipInput() {
  if (abilityTimer <= 0.0f) {
    return;
  }

  bool teleport = false;

  glm::vec3 cameraUp = glm::normalize(orientation * global::cameraUp);
  glm::vec3 cameraRight = glm::normalize(orientation * global::cameraRight);
  glm::vec3 cameraFront = glm::normalize(orientation * global::cameraFront);

  if (keys::actionPressed[keys::forward]) {
    position += cameraFront * ship::timeShip.abilityDistance;
    keys::actionPressed[keys::forward] = false;
    teleport = true;
  }

  if (keys::actionPressed[keys::backward]) {
    position -= cameraFront * ship::timeShip.abilityDistance;
    keys::actionPressed[keys::backward] = false;
    teleport = true;
  }

  if (keys::actionPressed[keys::yawLeft]) {
    position -= cameraRight * ship::timeShip.abilityDistance;
    keys::actionPressed[keys::yawLeft] = false;
    teleport = true;
  }
  if (keys::actionPressed[keys::yawRight]) {
    position += cameraRight * ship::timeShip.abilityDistance;
    keys::actionPressed[keys::yawRight] = false;
    teleport = true;
  }

  if (keys::actionPressed[keys::pitchUp]) {
    position += cameraUp * ship::timeShip.abilityDistance;
    keys::actionPressed[keys::pitchUp] = false;
    teleport = true;
  }
  if (keys::actionPressed[keys::pitchDown]) {
    position -= cameraUp * ship::timeShip.abilityDistance;
    keys::actionPressed[keys::pitchDown] = false;
    teleport = true;
  }

  if (teleport) {
    abilityTimer = 0.0f;
  }
}

void Player::speedShipUpdate() {
  if (ultimateTimer > 0.0f) {
    speedBoost = ship::speedShip.ultimateSpeedBoost;
    maxSpeed = ship::speedShip.ultimateMaxSpeed;
  } else {
    speedBoost = 1.0f;
    maxSpeed = ship::shipMaxSpeed[ship];
  }

  float max = ship::speedShip.passiveMaxDamageBoost;
  float x = speed / ship::speedShip.passiveMaxSpeed;
  float y = pow(max, pow(x, ship::speedShip.passiveStrength));

  damageBoost = y;
}

void Player::speedShipAbility() {
  if (abilityCounter < ship::speedShip.abilityCooldown)
    return;

  abilityCounter = 0;
  abilityTimer = ship::speedShip.abilityLength;
}

void Player::speedShipUltimate() {
  if (ultimateCounter < ship::speedShip.ultimateCooldown)
    return;

  ultimateCounter = 0;
  ultimateTimer = ship::speedShip.ultimateLength;
}

void Player::speedShipInput() {
  if (abilityTimer <= 0.0f) {
    return;
  }

  bool turn = false;

  glm::vec3 cameraUp = glm::normalize(orientation * global::cameraUp);
  glm::vec3 cameraRight = glm::normalize(orientation * global::cameraRight);
  glm::vec3 cameraFront = glm::normalize(orientation * global::cameraFront);

  if (keys::actionPressed[keys::yawLeft]) {
    glm::quat rotate =
        glm::angleAxis(glm::radians(ship::speedShip.abilityAngle), cameraUp);
    orientation = glm::normalize(rotate * orientation);
    keys::actionPressed[keys::yawLeft] = false;
    turn = true;
  }
  if (keys::actionPressed[keys::yawRight]) {
    glm::quat rotate =
        glm::angleAxis(glm::radians(-ship::speedShip.abilityAngle), cameraUp);
    orientation = glm::normalize(rotate * orientation);

    keys::actionPressed[keys::yawRight] = false;
    turn = true;
  }

  if (keys::actionPressed[keys::pitchUp]) {
    glm::quat rotate = glm::angleAxis(
        glm::radians(-ship::speedShip.abilityAngle), cameraRight);
    orientation = glm::normalize(rotate * orientation);

    keys::actionPressed[keys::pitchUp] = false;
    turn = true;
  }
  if (keys::actionPressed[keys::pitchDown]) {
    glm::quat rotate = glm::angleAxis(
        glm::radians(-ship::speedShip.abilityAngle), cameraRight);
    orientation = glm::normalize(rotate * orientation);

    keys::actionPressed[keys::pitchDown] = false;
    turn = true;
  }

  if (keys::actionPressed[keys::rollLeft]) {
    glm::quat rotate = glm::angleAxis(
        glm::radians(-ship::speedShip.abilityAngle), cameraFront);
    orientation = glm::normalize(rotate * orientation);

    keys::actionPressed[keys::rollLeft] = false;
    turn = true;
  }

  if (keys::actionPressed[keys::rollRight]) {
    glm::quat rotate = glm::angleAxis(
        glm::radians(-ship::speedShip.abilityAngle), cameraFront);
    orientation = glm::normalize(rotate * orientation);

    keys::actionPressed[keys::rollRight] = false;
    turn = true;
  }

  if (turn)
    abilityTimer = 0.0f;
}

void Player::parryShipAbility() {
  if (abilityCounter < ship::parryShip.abilityCooldown)
    return;

  glm::vec3 cameraFront = glm::normalize(orientation * global::cameraFront);
  for (unsigned int i = 0; i < projectiles.size(); i++) {
    Projectile &projectile = projectiles[i];
    if (projectile.bullet) {
      if (projectile.bullet->enemyBullet) {
        float distance = glm::distance(projectile.bullet->position, position);
        if (distance < ship::parryShip.abilityParryMaxDistance) {
          projectile.bullet->direction = cameraFront;
          projectile.bullet->enemyBullet = false;
          projectile.bullet->color = glm::vec3(1.0f);
          projectile.bullet->speed *= ship::parryShip.passiveParrySpeedBoost;
          projectile.bullet->damage *= ship::parryShip.passiveParryDamage;
        }
      }
    }
  }

  abilityCounter = 0;
}

void Player::parryShipUltimate() {
  if (ultimateCounter < ship::parryShip.ultimateCooldown)
    return;

  glm::vec3 cameraFront = glm::normalize(orientation * global::cameraFront);
  for (unsigned int i = 0; i < projectiles.size(); i++) {
    Projectile &projectile = projectiles[i];
    if (projectile.bullet) {
      if (projectile.bullet->enemyBullet) {
        projectile.bullet->direction = cameraFront;
        projectile.bullet->enemyBullet = false;
        projectile.bullet->color = glm::vec3(1.0f);
        projectile.bullet->speed *= ship::parryShip.passiveParrySpeedBoost;
        projectile.bullet->damage *= ship::parryShip.passiveParryDamage;
      }
    }
  }
  ultimateCounter = 0;
}

void Player::vampireShipUpdate() {

  if (ultimateTimer > 0.0f) {
    healShip(ship::vampireShip.ultimateHealAmount);
  }

  float finalDamage = 0.0f;
  if (keys::actionPressed[keys::ability] &&
      health - ship::vampireShip.abilityHealthCost >= 0.0f) {
    finalDamage += ship::vampireShip.abilityDamageBoost;
    takeDamage(ship::vampireShip.abilityHealthCost);
  }

  float max = ship::vampireShip.passiveMaxDamageBoost;
  float x = 1.0f - (health / ship::vampireShip.health);
  float y = pow(max, pow(x, ship::vampireShip.passiveStrength));

  finalDamage += y;

  damageBoost = finalDamage;
}

void Player::vampireShipAbility() {}

void Player::vampireShipUltimate() {
  if (ultimateCounter < ship::vampireShip.ultimateCooldown)
    return;

  ultimateTimer = ship::vampireShip.ultimateLength;
  ultimateCounter = 0;
}

void Player::takeDamage(float damage) {
  health -= damage * damageReductionAmount;
}

void Player::healShip(float addHealth) {
  if (health + addHealth > ship::shipMaxHealth[ship]) {
    health = ship::shipMaxHealth[ship];
  } else {
    health += addHealth;
  }
}

void Player::displayScreen(player::DisplayContext displayContext) {
  displayHealth(displayContext);
  displayArrow(displayContext);
  displayReload(displayContext);
  displayCooldown(displayContext);
  displayCooldownText(displayContext);
}

void Player::displayHealth(player::DisplayContext displayContext) {
  shader::shader.health->use();
  shader::shader.health->setFloat("uMaxHealth", ship::shipMaxHealth[ship]);
  shader::shader.health->setFloat("uCurrentHealth", health);
  shader::shader.health->setVec2f(
      "uResolution",
      glm::vec2(config::gameConfig.width, config::gameConfig.height));

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Player::displayArrow(player::DisplayContext displayContext) {
  glm::vec4 bossClip = displayContext.projection * displayContext.view *
                       glm::vec4(displayContext.bossPos, 1.0f);
  bossClip /= bossClip.w;

  bool onScreen = bossClip.x >= -1.0f && bossClip.x <= 1.0f &&
                  bossClip.y >= -1.0f && bossClip.y <= 1.0f &&
                  bossClip.z >= -1.0f && bossClip.z <= 1.0f;

  glm::vec3 toBoss = displayContext.bossPos - position;
  glm::vec3 bossDirView = glm::mat3(displayContext.view) * toBoss;

  glm::vec2 arrowDir = glm::normalize(glm::vec2(bossDirView));

  if (!onScreen) {
    shader::shader.arrow->use();
    shader::shader.arrow->setVec2f("uArrowPos", arrowDir);
    shader::shader.arrow->setVec2f(
        "uResolution",
        glm::vec2(config::gameConfig.width, config::gameConfig.height));

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }
}

void Player::displayReload(player::DisplayContext displayContext) {
  shader::shader.reload->use();

  float progress = shootCounter;
  if (weapons[weaponIndex] == player::swingBlade)
    progress = bullet::blade.spinTime - blade->spinCounter;

  shader::shader.reload->setFloat("uShootCounter", progress);

  float cooldown = bullet::bulletShootCooldown[weapons[weaponIndex]];

  shader::shader.reload->setFloat("uShootCooldown", cooldown);

  shader::shader.reload->setVec2f(
      "uResolution",
      glm::vec2(config::gameConfig.width, config::gameConfig.height));

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Player::displayCooldown(player::DisplayContext displayContext) {
  shader::shader.cooldown->use();

  float cooldown = ship::shipAbilityCooldown[ship];
  float timer = abilityCounter;
  shader::shader.cooldown->setFloat("uTimer", timer);
  shader::shader.cooldown->setFloat("uCooldown", cooldown);

  glm::vec2 translate = glm::vec2(-0.75, -0.95);
  shader::shader.cooldown->setVec2f("uTranslate", translate);

  shader::shader.cooldown->setVec2f(
      "uResolution",
      glm::vec2(config::gameConfig.width, config::gameConfig.height));

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  cooldown = ship::shipUltimateCooldown[ship];
  timer = ultimateCounter;
  shader::shader.cooldown->setFloat("uTimer", timer);
  shader::shader.cooldown->setFloat("uCooldown", cooldown);

  translate = glm::vec2(0.75, -0.95);
  shader::shader.cooldown->setVec2f("uTranslate", translate);

  // displayContext.cooldownShader->setVec2f(
  //     "uResolution",
  //     glm::vec2(config::gameConfig.width, config::gameConfig.height));

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Player::displayCooldownText(player::DisplayContext displayContext) {
  renderText(displayContext.textProjection, "test", 100.0f, 100.0f, 100.0f,
             glm::vec3(1.0f));
}
