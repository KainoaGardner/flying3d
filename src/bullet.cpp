#include "../include/bullet.h"

const float MACHINE_GUN_COOLDOWN = 0.05f;
const float MACHINE_GUN_SPEED = 5.0f;
const float MACHINE_GUN_SPREAD = 50.0f;
const float MACHINE_GUN_BULLET_SIZE = 0.5f;
const float MACHINE_GUN_DAMAGE = 1.0f;

const float SHOTGUN_COOLDOWN = 0.5f;
const float SHOTGUN_SPEED = 3.0f;
const float SHOTGUN_SPREAD = 5.0f;
const float SHOTGUN_BULLET_SIZE = 1.0f;
const float SHOTGUN_DAMAGE = 0.5f;

const float HOMING_MISSILE_COOLDOWN = 0.2f;
const float HOMING_MISSILE_SPEED = 1.0f;
const float HOMING_MISSILE_SPREAD = 10.0f;
const float HOMING_MISSILE_BULLET_SIZE = 0.75f;
const float HOMING_MISSILE_DAMAGE = 5.0f;

const float BOMB_LAUNCHER_COOLDOWN = 0.75f;
const float BOMB_LAUNCHER_SPEED = 2.0f;
const float BOMB_LAUNCHER_SPREAD = 20.0f;
const float BOMB_LAUNCHER_BULLET_SIZE = 3.0f;
const float BOMB_LAUNCHER_DAMAGE = 10.0f;
const float BOMB_LAUNCHER_EXPLOSION_TIMER = 2.0f;

const float CHARGE_RIFLE_COOLDOWN = 1.0f;
const float CHARGE_RIFLE_SPEED = 3.0f;
const float CHARGE_RIFLE_SPREAD = 100.0f;
const float CHARGE_RIFLE_BULLET_SIZE = 5.0f;
const float CHARGE_RIFLE_DAMAGE = 30.0f;

Bullet::Bullet(glm::vec3 positionIn, glm::vec3 rotationIn,
               glm::vec3 directionIn, glm::quat orientationIn,
               glm::vec3 scaleIn, glm::vec3 colorIn, float speedIn,
               float damageIn) {
  position = positionIn;

  direction = glm::normalize(directionIn);
  scale = scaleIn;
  speed = speedIn;
  color = colorIn;
  damage = damageIn;
  rotation = rotationIn;
  orientation = orientationIn;
}

void Bullet::update(float dt) { position += direction * speed; }

void Bullet::killBullet(glm::vec3 playerPosition) {
  float dist = glm::distance(playerPosition, position);
  if (dist > 1000.0f) {
    alive = false;
  }
}

void Bullet::draw(Shader shader, float timePassed) {
  glm::mat4 model = glm::mat4(1.0f);

  model = glm::translate(model, position);
  glm::vec3 pitchAxis = glm::rotate(orientation, glm::vec3(1.0f, 0.0f, 0.0f));
  glm::quat spinQuat =
      glm::angleAxis(timePassed * 25.0f, glm::normalize(pitchAxis));

  spinQuat = spinQuat * orientation;

  model *= glm::mat4_cast(spinQuat);

  model = glm::scale(model, scale);

  shader.setMatrix4fv("uModel", model);
  shader.setVec3f("uColor", color);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

BombBullet::BombBullet(glm::vec3 positionIn, glm::vec3 rotationIn,
                       glm::vec3 directionIn, glm::quat orientationIn,
                       glm::vec3 scaleIn, glm::vec3 colorIn, float speedIn,
                       float damageIn, float explodeTimerIn)
    : Bullet(positionIn, rotationIn, directionIn, orientationIn, scaleIn,
             colorIn, speedIn, damageIn) {
  explodeTimer = explodeTimerIn;
}

void BombBullet::update(float dt) {
  Bullet::update(dt);
  explodeCounter += dt;
  if (explodeCounter > explodeTimer) {
    explode();
  }
}

void BombBullet::explode() { alive = false; }

std::vector<Projectile> projectiles;
