#include "../include/display.h"
#include "../include/particle.h"

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
}


void DamageText::update() {
  disappearCounter += 1.0f;
  float k = 4.0f;

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

std::vector<ParticleList> particles;
std::vector<DamageText> damageTextParticles;
