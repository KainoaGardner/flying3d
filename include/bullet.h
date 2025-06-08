#ifndef BULLET_H
#define BULLET_H

#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/quaternion.hpp"
#include <GLFW/glfw3.h>
#include <string>

class Bullet {
public:
  glm::vec3 position;
  glm::vec3 rotation;
  glm::vec3 scale;
  glm::vec3 direction;

  float speed;
  float damage;

  bool alive = true;
  unsigned int viewDirection;

  Bullet(glm::vec3 positionIn, glm::vec3 rotationIn, glm::vec3 directionIn,
         glm::vec3 scaleIn, float speedIn, float damageIn);
  void update(float dt);

  void killBullet(glm::vec3 playerPosition);

private:
};

extern std::vector<Bullet> bullets;

#endif
