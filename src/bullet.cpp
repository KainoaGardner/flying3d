#include "../include/bullet.h"
Bullet::Bullet(glm::vec3 positionIn, glm::vec3 rotationIn,
               glm::vec3 directionIn, glm::vec3 scaleIn, float speedIn,
               float damageIn) {
  position = positionIn;

  direction = glm::normalize(directionIn);
  scale = scaleIn;
  speed = speedIn;

  damage = damageIn;

  rotation = rotationIn;
}

void Bullet::update(float dt) { position += direction * speed; }

void Bullet::killBullet(glm::vec3 playerPosition) {
  float dist = glm::distance(playerPosition, position);
  if (dist > 1000.0f) {
    alive = false;
  }
}

std::vector<Bullet> bullets;
