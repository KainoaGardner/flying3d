#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GLFW/glfw3.h>

extern float CUBE_VERTICES[120];
extern float CUBE_MAP_VERTICES[72];
extern float BEAM_VERTICES[120];

extern float SCREEN_VERTICES[20];

extern unsigned int CUBE_INDICES[36];
extern unsigned int SCREEN_INDICES[6];

struct Geometry {
  GLuint vao;
  GLuint vbo;
  GLuint ebo;
};

namespace geometry {
struct Geometries {
  Geometry cube;
  Geometry beam;
  Geometry skybox;
  Geometry screen;
  Geometry text;
};

extern Geometries geometry;
} // namespace geometry

Geometry createCube();
Geometry createCubemap();
Geometry createScreen();
Geometry createBeam();
Geometry createText();

#endif
