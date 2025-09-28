#include "../include/display.h"
#include "../include/geometry.h"
#include "../include/shader.h"
#include "../include/particle.h"

#include <iostream>


namespace particle {
Explosion explosion;
DamageText damageText;
}

Particle::Particle(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn) {
  position = positionIn;
  scale = scaleIn;
  orientation = orientationIn;
}

void Particle::draw() {};
void Particle::update() {};

Explosion::Explosion(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn,
                     float maxScaleIn, float explosionTimerIn) 
  : Particle(positionIn, orientationIn, scaleIn) {
  maxScale = maxScaleIn;
  explosionTimer = explosionTimerIn;
}


void Explosion::update() {
  explosionCounter += 1.0f;
  float k = 4.0f;

  float explosionAmount = explosionCounter / explosionTimer;
  float y = (1.0 - exp(k * (explosionAmount - 1.0f)));
  float size = maxScale * y;
  scale = glm::vec3(size);

  if (explosionCounter > explosionTimer) {
    alive = false;
  }
}

void Explosion::draw() {
  glBindVertexArray(geometry::geometry.cube.vao);
  glm::mat4 model = glm::mat4(1.0f);

  model = glm::translate(model, position);
  model *= glm::mat4_cast(orientation);
  model = glm::scale(model, scale);

  shader::shader.bullet->setMatrix4fv("uModel", model);
  shader::shader.bullet->setVec3f("uColor", glm::vec3(1.0f));

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

DamageText::DamageText(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn,float maxScaleIn, float disappearTimeIn,float damageIn) 
  : Particle(positionIn, orientationIn, scaleIn) {
  maxScale = maxScaleIn;
  damage = damageIn;
  disappearTime = disappearTimeIn;

  dir = glm::normalize(glm::vec3(rand() % 100 - 50,rand() % 100 - 50,rand() % 100 - 50));
}


void DamageText::update() {
  disappearCounter += 1.0f;
  float k = 4.0f;

  position += dir * particle::damageText.speed;

  float x = disappearCounter / disappearTime;
  float y = (1.0 - exp(k * (x - 1.0f)));
  float size = particle::damageText.size * y;
  scale = glm::vec3(size);

  if (disappearCounter > disappearTime) {
    alive = false;
  }
}

void DamageText::drawText(glm::mat4 view,glm::mat4 projection,glm::mat4 textProjection) {
  glm::vec4 clipSpace = projection * view * glm::vec4(position, 1.0f);
  glm::vec3 ndc = glm::vec3(clipSpace) / clipSpace.w;

  float x = config::gameConfig.width - (1.0f - (ndc.x * 0.5f + 0.5f)) * config::gameConfig.width;
  float y = ((ndc.y * 0.5f + 0.5f)) * config::gameConfig.height;
  float size = glm::max(0.2f,damage / 100.0f);
  renderText(textProjection, std::to_string(int(glm::ceil(damage))), x, y, size,
               glm::vec3(1.0f,1.0f,1.0f));

}

ZapLine::ZapLine(glm::vec3 startIn, glm::vec3 endIn,  float disappearTimeIn) {
  start = startIn;
  end = endIn;
  disappearTime = disappearTimeIn;
}

void ZapLine::update() {
  disappearCounter += 1.0f;

  if (disappearCounter > disappearTime) {
    alive = false;
  }
}

void ZapLine::draw(Player *player) {
  if (player == nullptr) {return;}


  glBindVertexArray(geometry::geometry.face.vao);

  glm::vec3 dir = end - start;
  float len = glm::length(dir);
  dir = glm::normalize(dir);

  glm::vec3 midPoint = 0.5f * (start + end);
  glm::vec3 toCamera = glm::normalize(player->position - midPoint);
  glm::vec3 side = glm::normalize(glm::cross(dir,toCamera));
  glm::vec3 forward = glm::normalize(glm::cross(side,dir));

  glm::vec3 offsetStart = start + dir * len * 0.5f;
  glm::mat4 model = glm::mat4(1.0f);

  float thickness = 5.0f;
  model[0] = glm::vec4(side * thickness, 0.0f);
  model[1] = glm::vec4(dir * len, 0.0f);
  model[2] = glm::vec4(forward,0.0f);
  model[3] = glm::vec4(offsetStart,1.0f);


  shader::shader.bullet->setMatrix4fv("uModel", model);
  shader::shader.bullet->setVec3f("uColor", glm::vec3(1.0f));

  glDrawElements(GL_TRIANGLES, 20, GL_UNSIGNED_INT, 0);
}


std::vector<ParticleList> particles;
std::vector<DamageText> damageTextParticles;
std::vector<ZapLine> zapLineParticles;
