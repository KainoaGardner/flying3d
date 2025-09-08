#ifndef PARTICLE_H
#define PARTICLE_H

#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/quaternion.hpp"

#include "../include/shader.h"
#include <GLFW/glfw3.h>
#include <memory>

namespace particle {
struct Explosion {
  const float timer = 100.0f;
  const float size = 30.0f;
};

struct DamageText {
  const float timer = 25.0f;
  const float size = 5.0f;
};


extern Explosion explosion;
extern DamageText damageText;
} // namespace particle


class Particle {
public:
  glm::vec3 position;
  glm::vec3 scale;
  glm::quat orientation;

  bool alive = true;

  Particle(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn);

  virtual void update();
  virtual void draw();

private:
};

class Explosion : public Particle {
public:
  float maxScale;
  float explosionTimer;

  Explosion(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn,float maxScaleIn,
            float explosionTimerIn);

  void update() override;
  void draw() override;

private:
  float explosionCounter = 0.0f;
};

class DamageText : public Particle {
public:

  float maxScale;
  float disappearTime;
  float damage;

  DamageText(glm::vec3 positionIn, glm::quat orientationIn, glm::vec3 scaleIn,float maxScaleIn,
            float disappearTime,float damageIn);

  void update() override;
  void drawText(glm::mat4 view,glm::mat4 projection,glm::mat4 textProjection);

private:
  float disappearCounter = 0.0f;
};

struct ParticleList {
  std::unique_ptr<Particle> particle;
};

extern std::vector<ParticleList> particles;
extern std::vector<DamageText> damageTextParticles;

#endif
