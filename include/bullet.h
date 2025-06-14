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
extern const float ZAP_RIFLE_ZAP_COOLDOWN;
extern const float ZAP_RIFLE_ZAP_RANGE;

extern const float CANNON_COOLDOWN;
extern const float CANNON_SPEED;
extern const float CANNON_SPREAD;
extern const float CANNON_BULLET_SIZE;
extern const float CANNON_DAMAGE;

extern const float LASER_COOLDOWN;
extern const float LASER_SIZE;
extern const float LASER_LENGTH;
extern const float LASER_DAMAGE;
extern const float LASER_SPIN_UP_TIME;
extern const float LASER_SPIN_STRENGTH;
extern const float LASER_MAX_SPIN_SPEED;

extern const float BLADE_SPIN_TIME;
extern const float BLADE_SIZE;

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

class ZapBullet : public Bullet {
public:
  ZapBullet(glm::vec3 positionIn, glm::vec3 rotationIn, glm::vec3 directionIn,
            glm::quat orientationIn, glm::vec3 scaleIn, glm::vec3 colorIn,
            float speedIn, float damageIn);

  void update(float dt) override;

private:
  float zapCounter = 0.0f;
};

// struct Projectile {
//   std::unique_ptr<Bullet> bullet;
//   std::unique_ptr<HomingMissile> homingMissile;
//   std::unique_ptr<BombBullet> bombBullet;
//   std::unique_ptr<ZapBullet> zapBullet;
// };

class Laser {
public:
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat orientation;

  glm::vec3 color;

  float damage;
  bool on = false;

  Laser(glm::vec3 colorIn, float damageIn);

  void update(float dt, glm::vec3 playerPos, glm::quat playerOrientation);

  void draw(Shader shader, float timePassed);

private:
  float spinCounter = 0.0f;
  float spinSpeed = 0.0f;
};

class Blade {
public:
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat orientation;

  glm::vec3 color;

  float damage;
  float spinCounter = 0.0f;

  Blade(glm::vec3 scaleIn, glm::vec3 colorIn, float damageIn);

  void update(float dt, glm::vec3 playerPos, glm::quat playerOrientation);
  void draw(Shader shader, float timePassed);

private:
};

struct Projectile {
  std::unique_ptr<Bullet> bullet;
};

extern std::vector<Projectile> projectiles;

#endif
