#ifndef SHIPS_H
#define SHIPS_H

namespace ship {
struct NormalShip {
  const float health = 1000.0f;
  const float maxSpeed = 4.0f;
  const float acceleration = 0.004f;
  const float breakStrength = 0.004f;
  const float turnSpeed = 1.3f;
  const float dragStrength = 0.002f;

  const float abilityCooldown = 1000.0f;
  const float abilitySpeed = 5.0f;
  const float abilityLength = 100.0f;
  const float abilityStrength = 4.0f;

  const float ultimateCooldown = 10000.0f;
  const float ultimateLength = 1000.0f;
  const float ultimateBoost = 2.0f;

  const float passiveStrength = -4.0f;
  const float maxPassiveTime = 5000.0f;
  const float maxPassiveBoost = 2.0f;
};

struct TankShip {
  const float health = 3000.0f;
  const float maxSpeed = 3.0f;
  const float acceleration = 0.002f;
  const float breakStrength = 0.002f;
  const float turnSpeed = 1.0f;
  const float dragStrength = 0.002f;

  const float abilityCooldown = 1500.0f;
  const float abilityDamageBlock = 0.3f;
  const float abilityLength = 500.0f;

  const float ultimateCooldown = 10000.0f;
  const float ultimateLength = 500.0f;
  const float ultimateBoost = 2.0f;

  const float passiveDamageBoost = 1.5f;
  const float passiveMinSpeed = 0.15f;
};

struct TimeShip {
  const float health = 750.0f;
  const float maxSpeed = 4.5f;
  const float acceleration = 0.003f;
  const float breakStrength = 0.003f;
  const float turnSpeed = 1.2f;
  const float dragStrength = 0.002f;

  const float abilityCooldown = 1500.0f;
  const float abilityLength = 100.0f;
  const float abilityDistance = 150.0f;

  const float ultimateCooldown = 15000.0f;
  const float ultimateLength = 1000.0f;

  const float passiveMaxTimeStop = 0.5f;
  const float passiveMaxDistance = 50.0f;
};

struct SpeedShip {
  const float health = 500.0f;
  const float maxSpeed = 5.0f;
  const float acceleration = 0.005f;
  const float breakStrength = 0.005f;
  const float turnSpeed = 1.5f;
  const float dragStrength = 0.002f;

  const float abilityCooldown = 1500.0f;
  const float abilityLength = 100.0f;
  const float abilityAngle = 90.0f;

  const float ultimateCooldown = 10000.0f;
  const float ultimateMaxSpeed = 10.0f;
  const float ultimateLength = 2500.0f;
  const float ultimateSpeedBoost = 2.0f;

  const float passiveStrength = 1.0f;
  const float passiveMaxSpeed = 10.0f;
  const float passiveMaxDamageBoost = 3.0f;
};

struct ParryShip {
  const float health = 1500.0f;
  const float maxSpeed = 3.5f;
  const float acceleration = 0.003f;
  const float breakStrength = 0.003f;
  const float turnSpeed = 1.2f;
  const float dragStrength = 0.002f;

  const float abilityCooldown = 1000.0f;
  const float abilityParryMaxDistance = 50.0f;

  const float ultimateCooldown = 10000.0f;

  const float passiveParryDamage = 10.0f;
  const float passiveParrySpeedBoost = 2.0f;
};

struct VampireShip {
  const float health = 2000.0f;
  const float maxSpeed = 4.0f;
  const float acceleration = 0.004f;
  const float breakStrength = 0.003f;
  const float turnSpeed = 1.3f;
  const float dragStrength = 0.002f;

  const float abilityParryMaxDistance = 50.0f;
  const float abilityDamageBoost = 1.0f;
  const float abilityHealthCost = 10.0f;

  const float ultimateCooldown = 10000.0f;
  const float ultimateLength = 1000.0f;
  const float ultimateHealAmount = 10.0f;

  const float passiveMaxDamageBoost = 3.0f;
  const float passiveStrength = 3.0f;
  const float passiveLifeStealPercent = 10.0f;
};

extern NormalShip normalShip;
extern TankShip tankShip;
extern TimeShip timeShip;
extern SpeedShip speedShip;
extern ParryShip parryShip;
extern VampireShip vampireShip;

extern const float shipMaxHealth[6];
extern const float shipMaxSpeed[6];
extern const float shipAcceleration[6];
extern const float shipBreakStrength[6];
extern const float shipTurnSpeed[6];
extern const float shipDragStrength[6];

extern const float shipAbilityCooldown[6];
extern const float shipUltimateCooldown[6];

} // namespace ship

#endif
