#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "../include/glad/glad.h"

extern float CUBE_VERTICES[120];
extern float CUBE_MAP_VERTICES[72];
extern unsigned int CUBE_INDICES[36];

struct Geometry {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
};

Geometry createCube();
Geometry createCubemap();

#endif
