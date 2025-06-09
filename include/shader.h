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

  void setVec2f(const std::string &name, glm::vec2 value) const;
  void setVec3f(const std::string &name, glm::vec3 value) const;

  void setMatrix4fv(const std::string &name, glm::mat4 value) const;
};

#endif
