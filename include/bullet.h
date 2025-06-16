#ifndef BULLET_H
#define BULLET_H

#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glad/glad.h"
#include "../include/glm/gtx/quaternion.hpp"
#include "../include/particle.h"
#include "../include/shader.h"
#include <GLFW/glfw3.h>
#include <memory>

namespace bullet {
struct MachineGun {
  const float cooldown = 0.05f;
  const float speed = 5.0f;
  const float spread = 50.0f;
  const float bulletSize = 0.5f;
  const float damage = 1.0f;
};

struct Shotgun {
  const float cooldown = 0.5f;
  const float speed = 3.0f;
  const float spread = 5.0f;
  const float bulletSize = 1.0f;
  const float damage = 0.5f;
};

struct HomingMissile {
  const float cooldown = 0.2f;
  const float speed = 2.0f;
  const float turnSpeed = 2.0f;
  const float spread = 10.0f;
  const float bulletSize = 0.75f;
  const float damage = 5.0f;
};

struct BombLauncher {
  const float cooldown = 0.75f;
  const float speed = 2.0f;
  const float spread = 20.0f;
  const float bulletSize = 3.0f;
  const float damage = 10.0f;
  const float explosionTimer = 2.0f;
};

struct ChargeRifle {
  const float cooldown = 1.0f;
  const float speed = 3.0f;
  const float spread = 100.0f;
  const float bulletSize = 5.0f;
  const float damage = 30.0f;
  const float strength = 4.0f;
};

struct ZapRifle {
  const float cooldown = 1.0f;
  const float speed = 1.0f;
  const float spread = 30.0f;
  const float bulletSize = 1.0f;
  const float damage = 1.0f;
  const float zapCooldown = 0.5f;
  const float zapRange = 10.0f;
};

struct Cannon {
  const float cooldown = 3.0f;
  const float speed = 3.0f;
  const float spread = 50.0f;
  const float bulletSize = 10.0f;
  const float damage = 50.0f;
};

struct Laser {
  const float cooldown = 0.1f;
  const float size = 1.0f;
  const float length = 100.0f;
  const float damage = 2.0f;
  const float spinUpTime = 10.0f;
  const float spinStrength = 10.0f;
  const float maxSpinSpeed = 50.0f;
};

struct Blade {
  const float spinTime = 0.5f;
  const float size = 0.05f;
  const float length = 100.0f;
  const float damage = 30.0f;
};

extern MachineGun machineGun;
extern Shotgun shotgun;
extern HomingMissile homingMissile;
extern BombLauncher bombLauncher;
extern ChargeRifle chargeRifle;
extern ZapRifle zapRifle;
extern Cannon cannon;
extern Laser laser;
extern Blade blade;
} // namespace bullet

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
