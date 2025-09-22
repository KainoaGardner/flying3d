#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "../include/glm/glm.hpp"

class Shader {
public:
  unsigned int program;
  Shader(const char *vertexPath, const char *fragmentPath);

  void use();

  void setFloat(const std::string &name, float value) const;
  void setInt(const std::string &name, int value) const;

  void setVec2f(const std::string &name, glm::vec2 value) const;
  void setVec3f(const std::string &name, glm::vec3 value) const;

  void setMatrix4fv(const std::string &name, glm::mat4 value) const;
};

namespace shader {
struct Shaders {
  Shader *normal;
  Shader *bullet;
  Shader *cubemap;
  Shader *arrow;
  Shader *health;
  Shader *reload;
  Shader *cooldown;
  Shader *screen;
  Shader *text;
};

extern Shaders shader;
} // namespace shader

#endif
