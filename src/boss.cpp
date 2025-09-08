#include "../include/boss.h"
#include "../include/display.h"
#include "../include/collision.h"
#include <iostream>

namespace boss {
Cube cube;


const float bossMaxHealth[1] = {
  boss::cube.health,
};

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

void Boss::displayScreen(player::DisplayContext displayContext) {
  if (!alive) return;

  glDisable(GL_DEPTH_TEST);
  displayHealth(displayContext);
  displayBossName(displayContext);
  glEnable(GL_DEPTH_TEST);
}

void Boss::displayBossName(player::DisplayContext displayContext) {
  float x = config::gameConfig.width / 2.0f;
  float y = config::gameConfig.height - config::gameConfig.height / 24.0f;

  renderText(displayContext.textProjection, "Cube", x, y, 0.50f, glm::vec3(1.0f));

}


void Boss::displayHealth(player::DisplayContext displayContext) {
  glBindVertexArray(geometry::geometry.screen.vao);

  shader::shader.health->use();
  shader::shader.health->setFloat("uMaxHealth", boss::bossMaxHealth[boss::cubeBoss]);
  shader::shader.health->setFloat("uCurrentHealth", health);
  shader::shader.health->setVec2f(
      "uResolution",
      glm::vec2(config::gameConfig.width, config::gameConfig.height));
  shader::shader.health->setVec2f( "uPosition", glm::vec2(0.0f,0.85f));
  shader::shader.health->setVec2f( "uScale", glm::vec2(2.0f,0.05f));


  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Boss::collisionUpdate(Player *player) {
  float damage = bulletCollisionUpdate();
  damage += laserCollisionUpdate(player);
  player->vampireLifeSteal(damage);
}


float Boss::bulletCollisionUpdate(){
  float damage = 0;
  for (auto it = projectiles.begin();it != projectiles.end();) {
    if (checkBulletCollsion(*it->bullet)){
      takeDamage(it->bullet->damage);
      damage += it->bullet->damage;

      DamageText damageTextParticle(it->bullet->position, it->bullet->orientation,glm::vec3(1.0f), 
                                                       it->bullet->scale.x * 2.0f,particle::damageText.timer,it->bullet->damage);
      damageTextParticles.push_back(std::move(damageTextParticle));
      it = projectiles.erase(it);
    }else{
      ++it;
    }
  }

  return damage;
}

float Boss::laserCollisionUpdate(Player *player){
  if (player->weapons[player->weaponIndex] != player::laserCannon || !player->laser->on){
    return 0;
  } 

  float damage = 0;
  glm::vec3 cameraFront = glm::normalize(player->orientation * global::cameraFront);
  glm::vec3 toBoss = glm::normalize(position - player->position);

  float dist = glm::length2(player->position - position);
  float distToleranceIncrease = dist / 10000.f;

  float dot = glm::dot(cameraFront,toBoss);
  float tolerance = cos(glm::radians(bullet::laser.toleranceDegrees));
  tolerance = glm::min(tolerance,tolerance * distToleranceIncrease);
  float speed = player->laser->spinSpeed / bullet::laser.maxSpinSpeed;


  if (dot > tolerance) {
    takeDamage(bullet::laser.damage * speed * speed);
    damage += bullet::laser.damage * speed * speed;
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
    ParticleList particle;
    particle.particle = std::make_unique<Explosion>(position, orientation,glm::vec3(0.0f),
                                                     particle::explosion.size,
                                                     particle::explosion.timer);
    particles.push_back(std::move(particle));
  }
}

Cube::Cube(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn,
           float healthIn)
    : Boss(positionIn, orientationIn, scaleIn, healthIn) {}


void Cube::update(Player *player) {
  if (!alive) return;
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
    projectile.bullet = std::make_unique<Bullet>(
        position, toPlayer, toPlayer, global::cameraOrientation,
        glm::vec3(5.0f), glm::vec3(1.0f, 0.0f, 0.0f), 3.0f, 10.0f, true);
    projectiles.push_back(std::move(projectile));
    shootCounter = 0.0f;
  }

  collisionUpdate(player);
}

void Cube::display() {
  if (!alive) return;

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


