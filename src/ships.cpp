#include "../include/ships.h"

namespace ship {

NormalShip normalShip;
TankShip tankShip;
TimeShip timeShip;

const float shipMaxHealth[6] = {
    ship::normalShip.health,
    ship::tankShip.health,
    ship::timeShip.health,
};

const float shipMaxSpeed[6] = {
    ship::normalShip.maxSpeed,
    ship::tankShip.maxSpeed,
    ship::timeShip.maxSpeed,
};

const float shipAcceleration[6] = {
    ship::normalShip.acceleration,
    ship::tankShip.acceleration,
    ship::timeShip.acceleration,
};

const float shipBreakStrength[6] = {
    ship::normalShip.breakStrength,
    ship::tankShip.breakStrength,
    ship::timeShip.breakStrength,
};

const float shipTurnSpeed[6] = {
    ship::normalShip.turnSpeed,
    ship::tankShip.turnSpeed,
    ship::timeShip.turnSpeed,
};

const float shipDragStrength[6] = {
    ship::normalShip.dragStrength,
    ship::tankShip.dragStrength,
    ship::timeShip.dragStrength,
};
} // namespace ship
