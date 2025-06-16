#include "../include/particle.h"

namespace particle {
Explosion explosion;
}

Explosion::Explosion(glm::vec3 positionIn, glm::quat orientationIn,
                     float maxScaleIn, float explosionTimerIn) {
  position = positionIn;
  orientation = orientationIn;
  maxScale = maxScaleIn;
  explosionTimer = explosionTimerIn;
}

void Explosion::update(float dt) {
  explosionCounter += dt;
  float k = 4.0f;

  float explosionAmount = explosionCounter / explosionTimer;
  float y = (1.0 - exp(k * (explosionAmount - 1.0f)));
  float size = maxScale * y;
  scale = glm::vec3(size);

  if (explosionCounter > explosionTimer) {
    alive = false;
  }
}

void Explosion::draw(Shader shader) {
  glm::mat4 model = glm::mat4(1.0f);

  model = glm::translate(model, position);
  model *= glm::mat4_cast(orientation);
  model = glm::scale(model, scale);

  shader.setMatrix4fv("uModel", model);
  shader.setVec3f("uColor", glm::vec3(1.0f));

  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

std::vector<Particle> particles;
