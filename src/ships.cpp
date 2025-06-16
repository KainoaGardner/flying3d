#include "../include/ships.h"

namespace ship {

NormalShip normalShip;

const float shipMaxHealth[6] = {ship::normalShip.health};
const float shipMaxSpeed[6] = {ship::normalShip.maxSpeed};
const float shipAcceleration[6] = {ship::normalShip.acceleration};
const float shipBreakStrength[6] = {ship::normalShip.breakStrength};
const float shipTurnSpeed[6] = {ship::normalShip.turnSpeed};
const float shipDragStrength[6] = {ship::normalShip.dragStrength};
} // namespace ship
