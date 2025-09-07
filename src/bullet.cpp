#include "../include/bullet.h"

#include "../include/boss.h"
#include <iostream>

namespace bullet {
MachineGun machineGun;
FlameThrower flameThrower;
Shotgun shotgun;
HomingMissile homingMissile;
BombLauncher bombLauncher;
ChargeRifle chargeRifle;
ZapRifle zapRifle;
Cannon cannon;
Laser laser;
Blade blade;

const float bulletShootCooldown[10] = {
    bullet::machineGun.cooldown,    bullet::shotgun.cooldown, bullet::flameThrower.cooldown,
    bullet::homingMissile.cooldown, bullet::bombLauncher.cooldown,
    bullet::chargeRifle.cooldown,   bullet::zapRifle.cooldown,
    bullet::cannon.cooldown,        bullet::laser.cooldown,
    bullet::blade.spinTime,
};

} // namespace bullet

Bullet::Bullet(glm::vec3 positionIn, glm::vec3 rotationIn,
               glm::vec3 directionIn, glm::quat orientationIn,
               glm::vec3 scaleIn, glm::vec3 colorIn, float speedIn,
               float damageIn, bool enemyBulletIn) {
  position = positionIn;

  direction = glm::normalize(directionIn);
  scale = scaleIn;
  speed = speedIn;
  color = colorIn;
  damage = damageIn;
  rotation = rotationIn;
  orientation = orientationIn;
  enemyBullet = enemyBulletIn;
}

void Bullet::update(float timeSlow) {
  spinAngle = spinAngle + 1.0f * timeSlow;
  if (spinAngle > 2 * M_PI) {
    spinAngle -= 2 * M_PI;
  }

  position += direction * speed * timeSlow;
}

void Bullet::killBullet(glm::vec3 playerPosition) {
  float dist = glm::distance(playerPosition, position);
  if (dist > 1000.0f) {
    alive = false;
  }
}

void Bullet::draw() {
  glm::mat4 model = glm::mat4(1.0f);

  model = glm::translate(model, position);
  glm::vec3 pitchAxis = glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::quat spinQuat =
      glm::angleAxis(spinAngle * 25.0f, glm::normalize(pitchAxis));

  spinQuat = spinQuat * orientation;

  model *= glm::mat4_cast(spinQuat);

  model = glm::scale(model, scale);

  shader::shader.bullet->setMatrix4fv("uModel", model);
  shader::shader.bullet->setVec3f("uColor", color);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

BombBullet::BombBullet(glm::vec3 positionIn, glm::vec3 rotationIn,
                       glm::vec3 directionIn, glm::quat orientationIn,
                       glm::vec3 scaleIn, glm::vec3 colorIn, float speedIn,
                       float damageIn, bool enemyBulletIn, float explodeTimerIn)
    : Bullet(positionIn, rotationIn, directionIn, orientationIn, scaleIn,
             colorIn, speedIn, damageIn, enemyBulletIn) {
  explodeTimer = explodeTimerIn;
}




void BombBullet::update(float timeSlow) {
  Bullet::update(timeSlow);
  explodeCounter += 1.0f;
  if (explodeCounter > explodeTimer) {
    explode();
  }
}

void BombBullet::explode() {
  alive = false;
  Particle particle;
  particle.explosion = std::make_unique<Explosion>(position, orientation,
                                                   particle::explosion.size,
                                                   particle::explosion.timer);
  particles.push_back(std::move(particle));
}

FlameBullet::FlameBullet(glm::vec3 positionIn, glm::vec3 rotationIn,
                       glm::vec3 directionIn, glm::quat orientationIn,
                       glm::vec3 scaleIn, glm::vec3 colorIn, float speedIn,
                       float damageIn, bool enemyBulletIn, float disappearTimerIn)
    : Bullet(positionIn, rotationIn, directionIn, orientationIn, scaleIn,
             colorIn, speedIn, damageIn, enemyBulletIn) {
  disappearTimer = disappearTimerIn;
}

void FlameBullet::update(float timeSlow) {
  Bullet::update(timeSlow);
  disappearCounter += 1.0f;
  if (disappearCounter > disappearTimer) {
    alive = false;
  }
}


HomingMissile::HomingMissile(glm::vec3 positionIn, glm::vec3 rotationIn,
                             glm::vec3 directionIn, glm::quat orientationIn,
                             glm::vec3 scaleIn, glm::vec3 colorIn,
                             float speedIn, float damageIn, bool enemyBulletIn)
    : Bullet(positionIn, rotationIn, directionIn, orientationIn, scaleIn,
             colorIn, speedIn, damageIn, enemyBulletIn) {}

void HomingMissile::update(float timeSlow) {
  // change to closest target pos
  glm::vec3 closestTargetPos = boss::bossPosition;

  glm::vec3 targetDirection = glm::normalize(closestTargetPos - position);
  direction = glm::normalize(
      glm::mix(direction, targetDirection, bullet::homingMissile.turnSpeed));

  Bullet::update(timeSlow);
}

ZapBullet::ZapBullet(glm::vec3 positionIn, glm::vec3 rotationIn,
                     glm::vec3 directionIn, glm::quat orientationIn,
                     glm::vec3 scaleIn, glm::vec3 colorIn, float speedIn,
                     float damageIn, bool enemyBulletIn)
    : Bullet(positionIn, rotationIn, directionIn, orientationIn, scaleIn,
             colorIn, speedIn, damageIn, enemyBulletIn) {}

void ZapBullet::update(float timeSlow) {
  zapCounter += 1.0f;

  bool zap = false;
  if (zapCounter > bullet::zapRifle.cooldown) {
    // for close zap
    // if ZAP_RIFLE_ZAP_RANGE
    // zap = true;
  }
  if (zap) {
    zapCounter = 0.0f;
  }

  Bullet::update(timeSlow);
}



Laser::Laser(glm::vec3 colorIn, float damageIn) {
  color = colorIn;
  damage = damageIn;
}

void Laser::update(glm::vec3 playerPos, glm::quat playerOrientation) {
  if (!on) {
    spinCounter = 0.0f;
    return;
  }

  if (spinCounter < 100.f) {
    spinCounter += 1.0f;
  }

  float x = spinCounter / bullet::laser.spinUpTime;
  float num = 1 - exp(-bullet::laser.spinStrength * x);
  float denom = 1 - exp(-bullet::laser.spinStrength);
  float y = bullet::laser.size * (num / denom);
  float ySpin = bullet::laser.maxSpinSpeed * (num / denom);
  spinSpeed = ySpin;

  float size = glm::clamp(y, 0.0f, bullet::laser.size);

  scale = glm::vec3(size);
  position = playerPos;
  orientation = playerOrientation;

  // check hit do damage
}

void Laser::draw(float timePassed) {
  if (!on)
    return;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  glm::vec3 pitchAxis = glm::rotate(orientation, glm::vec3(0.0f, 0.0f, 1.0f));
  glm::quat spinQuat =
      glm::angleAxis(timePassed * spinSpeed, glm::normalize(pitchAxis));

  spinQuat = spinQuat * orientation;

  // model *= glm::mat4_cast(orientation);
  model *= glm::mat4_cast(spinQuat);

  model = glm::scale(model, scale);

  shader::shader.bullet->setMatrix4fv("uModel", model);
  shader::shader.bullet->setVec3f("uColor", color);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

Blade::Blade(glm::vec3 scaleIn, glm::vec3 colorIn, float damageIn) {
  scale = scaleIn;
  color = colorIn;
  damage = damageIn;
}

void Blade::update(glm::vec3 playerPos, glm::quat playerOrientation) {
  if (spinCounter < 0.0f) {
    return;
  }

  position = playerPos;
  orientation = playerOrientation;

  // check hit do damage
}

void Blade::draw(float timePassed) {
  if (spinCounter <= 0.0f) {
    return;
  }

  // glm::vec3 cameraUp =
  //     glm::normalize(orientation * glm::vec3(0.0f, 1.0f, 0.0f));

  glm::vec3 cameraRight =
      glm::normalize(orientation * glm::vec3(1.0f, 0.0f, 0.0f));
  glm::vec3 xOffset = cameraRight * 0.3f;
  ;

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position + xOffset);
  glm::vec3 pitchAxis = glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));

  float angle = 2 * M_PI * (spinCounter / bullet::blade.spinTime) + M_PI / 2.0;

  glm::quat spinQuat = glm::angleAxis(angle, glm::normalize(pitchAxis));
  spinQuat = spinQuat * orientation;

  // model *= glm::mat4_cast(orientation);
  model *= glm::mat4_cast(spinQuat);

  model = glm::scale(model, scale);

  shader::shader.bullet->setMatrix4fv("uModel", model);
  shader::shader.bullet->setVec3f("uColor", color);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  model = glm::mat4(1.0f);
  model = glm::translate(model, position - xOffset);
  pitchAxis = glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));

  spinQuat = glm::angleAxis(angle, glm::normalize(pitchAxis));
  spinQuat = spinQuat * orientation;
  model *= glm::mat4_cast(spinQuat);

  model = glm::scale(model, scale);

  shader::shader.bullet->setMatrix4fv("uModel", model);
  shader::shader.bullet->setVec3f("uColor", color);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

std::vector<Projectile> projectiles;
