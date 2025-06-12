#ifndef BULLET_H
#define BULLET_H

#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/quaternion.hpp"
#include "../include/particle.h"
#include "../include/shader.h"
#include <GLFW/glfw3.h>
#include <memory>

extern const float MACHINE_GUN_COOLDOWN;
extern const float MACHINE_GUN_SPEED;
extern const float MACHINE_GUN_SPREAD;
extern const float MACHINE_GUN_BULLET_SIZE;
extern const float MACHINE_GUN_DAMAGE;

extern const float SHOTGUN_COOLDOWN;
extern const float SHOTGUN_SPEED;
extern const float SHOTGUN_SPREAD;
extern const float SHOTGUN_BULLET_SIZE;
extern const float SHOTGUN_DAMAGE;

extern const float HOMING_MISSILE_COOLDOWN;
extern const float HOMING_MISSILE_SPEED;
extern const float HOMING_MISSILE_TURN_SPEED;
extern const float HOMING_MISSILE_SPREAD;
extern const float HOMING_MISSILE_BULLET_SIZE;
extern const float HOMING_MISSILE_DAMAGE;

extern const float BOMB_LAUNCHER_COOLDOWN;
extern const float BOMB_LAUNCHER_SPEED;
extern const float BOMB_LAUNCHER_SPREAD;
extern const float BOMB_LAUNCHER_BULLET_SIZE;
extern const float BOMB_LAUNCHER_DAMAGE;
extern const float BOMB_LAUNCHER_EXPLOSION_TIMER;

extern const float CHARGE_RIFLE_COOLDOWN;
extern const float CHARGE_RIFLE_SPEED;
extern const float CHARGE_RIFLE_SPREAD;
extern const float CHARGE_RIFLE_BULLET_SIZE;
extern const float CHARGE_RIFLE_DAMAGE;

extern const float ZAP_RIFLE_COOLDOWN;
extern const float ZAP_RIFLE_SPEED;
extern const float ZAP_RIFLE_SPREAD;
extern const float ZAP_RIFLE_BULLET_SIZE;
extern const float ZAP_RIFLE_DAMAGE;

extern const float CANNON_COOLDOWN;
extern const float CANNON_SPEED;
extern const float CANNON_SPREAD;
extern const float CANNON_BULLET_SIZE;
extern const float CANNON_DAMAGE;

class Bullet {
public:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  glm::vec3 direction;
  glm::quat orientation;

  glm::vec3 color;

  float speed;
  float damage;

  bool alive = true;

  Bullet(glm::vec3 positionIn, glm::vec3 rotationIn, glm::vec3 directionIn,
         glm::quat orientationIn, glm::vec3 scaleIn, glm::vec3 colorIn,
         float speedIn, float damageIn);
  virtual void update(float dt);

  void killBullet(glm::vec3 playerPosition);

  void draw(Shader shader, float timePassed);

private:
};

class BombBullet : public Bullet {
public:
  float explodeTimer;
  float explodeCounter = 0.0f;
  BombBullet(glm::vec3 positionIn, glm::vec3 rotationIn, glm::vec3 directionIn,
             glm::quat orientationIn, glm::vec3 scaleIn, glm::vec3 colorIn,
             float speedIn, float damageIn, float explodeTimerIn);

  void update(float dt) override;

  void explode();

private:
};

class HomingMissile : public Bullet {
public:
  HomingMissile(glm::vec3 positionIn, glm::vec3 rotationIn,
                glm::vec3 directionIn, glm::quat orientationIn,
                glm::vec3 scaleIn, glm::vec3 colorIn, float speedIn,
                float damageIn);

  void update(float dt) override;

private:
};

struct Projectile {
  std::unique_ptr<Bullet> bullet;
  std::unique_ptr<HomingMissile> homingMissile;
  std::unique_ptr<BombBullet> bombBullet;
};

extern std::vector<Projectile> projectiles;

#endif
