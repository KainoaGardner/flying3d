#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GLFW/glfw3.h>

namespace keys {
struct Gameplay {
  unsigned int forward;
  unsigned int backward;
  unsigned int pitchUp;
  unsigned int pitchDown;
  unsigned int yawRight;
  unsigned int yawLeft;
  unsigned int rollRight;
  unsigned int rollLeft;

  unsigned int shoot;
  unsigned int switchWeapon;
  unsigned int ability;
  unsigned int ultimate;
};

struct Camera {
  unsigned int back;
  unsigned int thirdPerson;
  unsigned int boss;
};

extern Gameplay gameplay;
extern Camera camera;

} // namespace keys

#endif
