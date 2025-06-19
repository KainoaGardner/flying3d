#ifndef KEY_H
#define KEY_H

#include <GLFW/glfw3.h>
#include <unordered_map>

namespace keys {
enum actions {
  forward,
  backward,
  pitchUp,
  pitchDown,
  yawRight,
  yawLeft,
  rollRight,
  rollLeft,
  shoot,
  switchWeapon,
  ability,
  ultimate,
  backCamera,
  thirdPersonCamera,
  bossCamera,
};

extern std::unordered_map<unsigned int, actions> keyToAction;
extern std::unordered_map<actions, bool> actionPressed;

} // namespace keys

#endif
