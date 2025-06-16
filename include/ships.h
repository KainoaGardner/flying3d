#ifndef SHIPS_H
#define SHIPS_H

#include <iostream>

namespace ship {
struct NormalShip {
  const float health = 1000.0f;
  const float maxSpeed = 300.0f;
  const float acceleration = 0.2f;
  const float breakStrength = 0.2f;
  const float turnSpeed = 100.0f;
  const float dragStrength = 0.2f;

  const float abilityCooldown = 10.0f;
  const float abilitySpeed = 500.0f;
  const float abilityLength = 1.0f;
  const float abilityStrength = 4.0f;

  const float ultimateCooldown = 100.0f;
  const float ultimateLength = 10.0f;
  const float ultimateBoost = 2.0f;

  const float passiveStrength = -4.0f;
  const float maxPassiveTime = 50.0f;
  const float maxPassiveBoost = 2.0f;
};

extern NormalShip normalShip;

extern const float shipMaxHealth[6];
extern const float shipMaxSpeed[6];
extern const float shipAcceleration[6];
extern const float shipBreakStrength[6];
extern const float shipTurnSpeed[6];
extern const float shipDragStrength[6];

} // namespace ship

#endif
