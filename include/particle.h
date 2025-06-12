#ifndef PARTICLE_H
#define PARTICLE_H

#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtx/quaternion.hpp"

#include "../include/shader.h"
#include <GLFW/glfw3.h>
#include <memory>

extern const float EXPLOSION_TIMER;
extern const float EXPLOSION_SIZE;

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

  void update(float dt);

  void draw(Shader shader);

private:
  float explosionCounter = 0.0f;
};

struct Particle {
  std::unique_ptr<Explosion> explosion;
};

extern std::vector<Particle> particles;

#endif
