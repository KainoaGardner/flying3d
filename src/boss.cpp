#include "../include/boss.h"
#include "../include/collision.h"
#include <iostream>

namespace boss {
Cube cube;

glm::vec3 bossPosition;
} // namespace boss

Boss::Boss(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn,
           float healthIn) {
  position = positionIn;
  orientation = orientationIn;
  scale = scaleIn;
  health = healthIn;
}

void Boss::update(Player *player) {}

void Boss::display() {}

void Boss::collisionUpdate(Player *player) {
  float damage = bulletCollisionUpdate();
  player->vampireLifeSteal(damage);
}


float Boss::bulletCollisionUpdate(){
  float damage = 0;
  for (auto it = projectiles.begin();it != projectiles.end();) {
    if (checkBulletCollsion(*it->bullet)){
      takeDamage(it->bullet->damage);
      damage += it->bullet->damage;
      it = projectiles.erase(it);
    }else{
      ++it;
    }
  }

  return damage;
}



bool Boss::checkBulletCollsion(Bullet& bullet) {
  if (bullet.enemyBullet) return false;

  collision::OBB a;
  collision::OBB b;

  glm::vec3 bossRight = glm::normalize(orientation * global::localRight);
  glm::vec3 bossUp = glm::normalize(orientation * global::localUp);
  glm::vec3 bossFront = glm::normalize(orientation * global::localFront);

  glm::vec3 bulletRight = glm::normalize(bullet.orientation * global::localRight);
  glm::vec3 bulletUp = glm::normalize(bullet.orientation * global::localUp);
  glm::vec3 bulletFront = glm::normalize(bullet.orientation * global::localFront);

  a.axes[0] = bossRight;
  a.axes[1] = bossUp;
  a.axes[2] = bossFront;
  a.center = position;
  a.halfSize = scale * 0.75f;

  b.axes[0] = bulletRight;
  b.axes[1] = bulletUp;
  b.axes[2] = bulletFront;

  b.center = bullet.position;
  b.halfSize = bullet.scale * 0.5f;

  return checkOBBCollision(a,b);
}

void Boss::takeDamage(float damage) {
  health -= damage;
  if (health < 0.0f && alive) {
    alive = false;
    Particle particle;
    particle.explosion = std::make_unique<Explosion>(position, orientation,
                                                     particle::explosion.size,
                                                     particle::explosion.timer);
    particles.push_back(std::move(particle));
  }
}

Cube::Cube(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn,
           float healthIn)
    : Boss(positionIn, orientationIn, scaleIn, healthIn) {}


void Cube::update(Player *player) {
  // position += glm::vec3(0.1f, 0.0f, 0.0f);

  // glm::vec3 cameraUp = glm::normalize(orientation * global::cameraUp);
  // glm::mat4 lookAt = glm::lookAt(position, player->position, cameraUp);
  // glm::quat targetOrientation = glm::quat_cast(glm::inverse(lookAt));
  //
  // orientation = glm::normalize(
  //     glm::mix(orientation, targetOrientation, boss::cube.turnSpeed));

  shootCounter += 1.0f * player->timeSlowAmount;
  if (shootCounter >= boss::cube.shootCooldown) {
    glm::vec3 toPlayer = glm::normalize(player->position - position);
    toPlayer.x += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;
    toPlayer.y += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;
    toPlayer.z += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;

    Projectile projectile;
    // projectile.bullet = std::make_unique<Bullet>(
    //     position, toPlayer, toPlayer, global::cameraOrientation,
    //     glm::vec3(5.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 100.0f, true);
    // projectiles.push_back(std::move(projectile));
    shootCounter = 0.0f;
  }

  collisionUpdate(player);
  std::cout << "Health: " << health << std::endl;
}

void Cube::display() {
  glBindVertexArray(geometry::geometry.cube.vao);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures::texture.cube);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, position);
  model *= glm::mat4_cast(orientation);
  model = glm::scale(model, scale);

  shader::shader.normal->use();
  shader::shader.normal->setMatrix4fv("uModel", model);

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}


