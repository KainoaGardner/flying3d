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
  const float shootCooldown = 10.0f;
  const float health = 10000.0f;
  const float turnSpeed = 0.05f;
};

extern glm::vec3 bossPosition;

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

private:
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
