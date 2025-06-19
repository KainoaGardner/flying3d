#include "../include/key.h"

namespace keys {

std::unordered_map<unsigned int, actions> keyToAction = {
    {GLFW_KEY_W, forward},    {GLFW_KEY_S, backward},
    {GLFW_KEY_A, yawLeft},    {GLFW_KEY_D, yawRight},
    {GLFW_KEY_I, pitchUp},    {GLFW_KEY_K, pitchDown},
    {GLFW_KEY_L, rollRight},  {GLFW_KEY_J, rollLeft},

    {GLFW_KEY_SPACE, shoot},  {GLFW_KEY_TAB, switchWeapon},
    {GLFW_KEY_Q, ability},    {GLFW_KEY_E, ultimate},

    {GLFW_KEY_1, backCamera}, {GLFW_KEY_2, thirdPersonCamera},
    {GLFW_KEY_3, bossCamera},
};

std::unordered_map<actions, bool> actionPressed = {
    {forward, false},      {backward, false},          {yawLeft, false},
    {yawRight, false},     {pitchUp, false},           {pitchDown, false},
    {rollRight, false},    {rollLeft, false},          {shoot, false},
    {switchWeapon, false}, {ability, false},           {ultimate, false},
    {backCamera, false},   {thirdPersonCamera, false}, {bossCamera, false},
};

} // namespace keys
