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

extern Explosion explosion;
} // namespace particle

class Explosion {
public:
  glm::vec3 position;
  glm::vec3 scale = glm::vec3(0.0f);
  glm::quat orientation;

  float maxScale;
  float explosionTimer;

  bool alive = true;

  Explosion(glm::vec3 positionIn, glm::quat orientationIn, float maxScaleIn,
            float explosionTimerIn);

  void update();

  void draw(Shader *shader);

private:
  float explosionCounter = 0.0f;
};

struct Particle {
  std::unique_ptr<Explosion> explosion;
};

extern std::vector<Particle> particles;

#endif
