#include "../include/ships.h"

namespace ship {

NormalShip normalShip;
TankShip tankShip;
TimeShip timeShip;
SpeedShip speedShip;
ParryShip parryShip;
VampireShip vampireShip;

const float shipMaxHealth[6] = {
    ship::normalShip.health, ship::tankShip.health,  ship::timeShip.health,
    ship::speedShip.health,  ship::parryShip.health, ship::vampireShip.health,
};

const float shipMaxSpeed[6] = {
    ship::normalShip.maxSpeed, ship::tankShip.maxSpeed,
    ship::timeShip.maxSpeed,   ship::speedShip.maxSpeed,
    ship::parryShip.maxSpeed,  ship::vampireShip.maxSpeed,
};

const float shipAcceleration[6] = {
    ship::normalShip.acceleration, ship::tankShip.acceleration,
    ship::timeShip.acceleration,   ship::speedShip.acceleration,
    ship::parryShip.acceleration,  ship::vampireShip.acceleration,
};

const float shipBreakStrength[6] = {
    ship::normalShip.breakStrength, ship::tankShip.breakStrength,
    ship::timeShip.breakStrength,   ship::speedShip.breakStrength,
    ship::parryShip.breakStrength,  ship::vampireShip.breakStrength,
};

const float shipTurnSpeed[6] = {
    ship::normalShip.turnSpeed, ship::tankShip.turnSpeed,
    ship::timeShip.turnSpeed,   ship::speedShip.turnSpeed,
    ship::parryShip.turnSpeed,  ship::vampireShip.turnSpeed,
};

const float shipDragStrength[6] = {
    ship::normalShip.dragStrength, ship::tankShip.dragStrength,
    ship::timeShip.dragStrength,   ship::speedShip.dragStrength,
    ship::parryShip.dragStrength,  ship::vampireShip.dragStrength,
};

extern const float shipAbilityCooldown[6]{
    ship::normalShip.abilityCooldown, ship::tankShip.abilityCooldown,
    ship::timeShip.abilityCooldown,   ship::speedShip.abilityCooldown,
    ship::parryShip.abilityCooldown,  0.0f,
};

extern const float shipUltimateCooldown[6]{
    ship::normalShip.ultimateCooldown, ship::tankShip.ultimateCooldown,
    ship::timeShip.ultimateCooldown,   ship::speedShip.ultimateCooldown,
    ship::parryShip.ultimateCooldown,  ship::vampireShip.ultimateCooldown,
};

} // namespace ship
