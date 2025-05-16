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

  void setMatrix4fv(const std::string &name, glm::mat4 value) const;
};

#endif
