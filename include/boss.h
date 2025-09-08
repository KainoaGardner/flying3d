#ifndef BOSS_H
#define BOSS_H

#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glad/glad.h"
#include "../include/glm/gtx/quaternion.hpp"
#include "../include/player.h"
#include "../include/textures.h"
#include <GLFW/glfw3.h>

namespace boss {
struct Cube {
  const float shootCooldown = 5.0f;
  const float health = 1000.0f;
  const float turnSpeed = 0.05f;
};

enum Ships {
  cubeBoss,
};


extern glm::vec3 bossPosition;

extern const float bossMaxHealth[1];

extern Cube cube;
} // namespace boss

class Boss {
public:
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat orientation;

  float health;

  bool alive = true;

  Boss(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn,
       float healthIn);

  virtual void update(Player *player);

  virtual void display();
  void displayScreen(player::DisplayContext);

private:

protected:
  void displayBossName(player::DisplayContext displayContext);
  void displayHealth(player::DisplayContext displayContext);

  void collisionUpdate(Player *player);
  float bulletCollisionUpdate();
  float laserCollisionUpdate(Player *player);
  bool checkBulletCollsion(Bullet& bullet); 
  void takeDamage(float damage);

};

class Cube : public Boss {
public:
  Cube(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn,
       float healthIn);

  void update(Player *player) override;
  void display() override;

  float shootCounter = 0.0f;

private:

};
#endif
