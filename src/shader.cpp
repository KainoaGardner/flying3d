#include "../include/shader.h"
#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::string vertexSourceCode;
  std::string fragmentSourceCode;
  std::ifstream vertexShaderFile;
  std::ifstream fragmentShaderFile;

  vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try {
    vertexShaderFile.open(vertexPath);
    fragmentShaderFile.open(fragmentPath);
    std::stringstream vertexShaderStream, fragmentShaderStream;

    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();

    vertexShaderFile.close();
    fragmentShaderFile.close();

    vertexSourceCode = vertexShaderStream.str();
    fragmentSourceCode = fragmentShaderStream.str();

  } catch (const std::ifstream::failure &e) {
    std::cout << "ERROR: Shader file not succesfully read" << std::endl;
  }

  const char *vertexShaderSourceCode = vertexSourceCode.c_str();
  const char *fragmentShaderSourceCode = fragmentSourceCode.c_str();

  unsigned int vertexShader, fragmentShader;
  int status;
  char infoLog[512];

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSourceCode, NULL);
  glCompileShader(vertexShader);

  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (!status) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "ERROR: Vertex shader Failed\n" << infoLog << std::endl;
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSourceCode, NULL);
  glCompileShader(fragmentShader);

  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
  if (!status) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR: fragment shader Failed\n" << infoLog << std::endl;
  }

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  glGetProgramiv(program, GL_LINK_STATUS, &status);
  if (!status) {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    std::cout << "ERROR:Program Link Failed\n" << infoLog << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
};

void Shader::use() { glUseProgram(program); };

void Shader::setFloat(const std::string &name, float value) const {
  int uniformLocation = glGetUniformLocation(program, name.c_str());
  glUniform1f(uniformLocation, value);
};

void Shader::setInt(const std::string &name, int value) const {
  int uniformLocation = glGetUniformLocation(program, name.c_str());
  glUniform1i(uniformLocation, value);
};

void Shader::setMatrix4fv(const std::string &name, glm::mat4 value) const {
  int uniformLocation = glGetUniformLocation(program, name.c_str());
  glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
};

void Shader::setVec3f(const std::string &name, glm::vec3 value) const {
  int uniformLocation = glGetUniformLocation(program, name.c_str());
  glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
}

void Shader::setVec2f(const std::string &name, glm::vec2 value) const {
  int uniformLocation = glGetUniformLocation(program, name.c_str());
  glUniform2fv(uniformLocation, 1, glm::value_ptr(value));
}

namespace shader {
Shaders shader;
}
