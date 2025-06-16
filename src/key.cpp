#include "../include/key.h"

namespace keys {
Gameplay gameplay = {
    .forward = GLFW_KEY_W,
    .backward = GLFW_KEY_S,
    .pitchUp = GLFW_KEY_I,
    .pitchDown = GLFW_KEY_K,
    .yawRight = GLFW_KEY_D,
    .yawLeft = GLFW_KEY_A,
    .rollRight = GLFW_KEY_L,
    .rollLeft = GLFW_KEY_J,

    .shoot = GLFW_KEY_SPACE,
    .switchWeapon = GLFW_KEY_TAB,
    .ability = GLFW_KEY_Q,
    .ultimate = GLFW_KEY_E,
};

Camera camera = {
    .back = GLFW_KEY_1,
    .thirdPerson = GLFW_KEY_2,
    .boss = GLFW_KEY_3,
};

} // namespace keys
