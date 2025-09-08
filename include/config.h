#ifndef CONFIG_H
#define CONFIG_H

#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/quaternion.hpp"
#include <GLFW/glfw3.h>
#include <limits>

namespace global {
const float maxCounter = std::numeric_limits<float>::max();
const glm::vec3 localFront = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 localUp = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 localRight = glm::vec3(1.0f, 0.0f, 0.0f);

const glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::quat cameraOrientation = glm::quat(1, 0, 0, 0);

} // namespace global

namespace config {
struct GameConfig {
  const unsigned int fps = 60.0;
  const float logicIntervalTime = 1.0f / fps;
  float lastUpdateTime = glfwGetTime();
  // unsigned int width = 1280;
  // unsigned int height = 720;
  unsigned int width = 1280;
  unsigned int height = 720;

};

extern GameConfig gameConfig;
} // namespace config

#endif
