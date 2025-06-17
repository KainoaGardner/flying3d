#include "../include/ships.h"

namespace ship {

NormalShip normalShip;
TankShip tankShip;
TimeShip timeShip;
SpeedShip speedShip;

const float shipMaxHealth[6] = {
    ship::normalShip.health,
    ship::tankShip.health,
    ship::timeShip.health,
    ship::speedShip.health,
};

const float shipMaxSpeed[6] = {
    ship::normalShip.maxSpeed,
    ship::tankShip.maxSpeed,
    ship::timeShip.maxSpeed,
    ship::speedShip.maxSpeed,
};

const float shipAcceleration[6] = {
    ship::normalShip.acceleration,
    ship::tankShip.acceleration,
    ship::timeShip.acceleration,
    ship::speedShip.acceleration,
};

const float shipBreakStrength[6] = {
    ship::normalShip.breakStrength,
    ship::tankShip.breakStrength,
    ship::timeShip.breakStrength,
    ship::speedShip.breakStrength,
};

const float shipTurnSpeed[6] = {
    ship::normalShip.turnSpeed,
    ship::tankShip.turnSpeed,
    ship::timeShip.turnSpeed,
    ship::speedShip.turnSpeed,
};

const float shipDragStrength[6] = {
    ship::normalShip.dragStrength,
    ship::tankShip.dragStrength,
    ship::timeShip.dragStrength,
    ship::speedShip.dragStrength,
};
} // namespace ship
