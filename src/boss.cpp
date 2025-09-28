#include "../include/display.h"
#include "../include/boss.h"
#include "../include/geometry.h"
#include "../include/collision.h"
#include "../include/shader.h"
#include "../include/textures.h"
#include "../include/particle.h"
#include "../include/bullet.h"

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

void Boss::displayScreen(config::DisplayContext displayContext) {
  if (!alive) return;

  glDisable(GL_DEPTH_TEST);
  displayHealth(displayContext);
  displayBossName(displayContext);
  glEnable(GL_DEPTH_TEST);
}

void Boss::displayBossName(config::DisplayContext displayContext) {
  float x = config::gameConfig.width / 2.0f;
  float y = config::gameConfig.height - config::gameConfig.height / 24.0f;

  renderText(displayContext.textProjection, "Cube", x, y, 0.50f, glm::vec3(1.0f));

}


void Boss::displayHealth(config::DisplayContext displayContext) {
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
  damage += bladeCollisionUpdate(player);
  player->vampireLifeSteal(damage);
}

float Boss::bulletCollisionUpdate(){
  float damage = 0;

  glm::vec2 errorSize(0.25f,0.0f);
  for (auto& p : projectiles){
    if (p.bullet->enemyBullet){continue;}

    if (setupOBBCollision(
      orientation,
      p.bullet->orientation,
      position,
      p.bullet->position,
      scale,
      p.bullet->scale,
      errorSize)){

      takeDamage(p.bullet->damage);
      damage += p.bullet->damage;

      p.bullet->killBullet();

    }
  }

  return damage;
}



float Boss::laserCollisionUpdate(Player *player){
  if (player->weapons[player->weaponIndex] != player::laserCannon){
    return 0;
  } 

  if (player->laser == nullptr || !player->laser->on || player->laser->spinSpeed < 15.0f){
    return 0;
  }

  glm::mat4 model = glm::translate(glm::mat4(1.0f),position);
  model *= glm::toMat4(orientation);
  model *= glm::scale(glm::mat4(1.0f),scale);

  glm::vec3 rayOrigin = player->position;
  glm::vec3 rayDir = glm::normalize(player->orientation * global::cameraFront);
  glm::vec3 worldHit;

  bool hit = checkRayOBBCollision(rayOrigin,rayDir,model,worldHit);

  float damage = 0;
  float speed = player->laser->spinSpeed / bullet::laser.maxSpinSpeed;

  if (hit) {
    takeDamage(bullet::laser.damage * speed * speed);
    damage += bullet::laser.damage * speed * speed;

    DamageText damageTextParticle(worldHit, orientation,glm::vec3(1.0f), 
                                                     bullet::laser.size,particle::damageText.timer,damage);
    damageTextParticles.push_back(std::move(damageTextParticle));
  }


  return damage;
}

float Boss::bladeCollisionUpdate(Player *player){
  if (player->weapons[player->weaponIndex] != player::swingBlade || player->blade == nullptr || player->blade->spinCounter < 0.0f){
    return 0;
  } 
  float damage = 0;

  glm::vec2 errorSize(0.25f,0.25f);

  if (setupOBBCollision(
    orientation,
    player->blade->orientation,
    position,
    player->blade->position,
    scale,
    player->blade->scale,
    errorSize)){
    takeDamage(player->blade->damage);
    damage += player->blade->damage;

    DamageText damageTextParticle(player->blade->position, orientation,glm::vec3(1.0f), 
                                                     bullet::blade.size,particle::damageText.timer,damage);
    damageTextParticles.push_back(std::move(damageTextParticle));
  }

  return damage;
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

  // shootCounter += 1.0f * player->timeSlowAmount;
  // if (shootCounter >= boss::cube.shootCooldown) {
  //   glm::vec3 toPlayer = glm::normalize(player->position - position);
  //   toPlayer.x += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;
  //   toPlayer.y += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;
  //   toPlayer.z += ((float(rand() % 100) / 100.0) - 0.5) / 5.0;
  //
  //   Projectile projectile;
  //   projectile.bullet = std::make_unique<Bullet>(
  //       position, toPlayer, toPlayer, global::cameraOrientation,
  //       glm::vec3(5.0f), glm::vec3(1.0f, 0.0f, 0.0f), 3.0f, 10.0f, true);
  //   projectiles.push_back(std::move(projectile));
  //   shootCounter = 0.0f;
  // }

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


