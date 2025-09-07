#ifndef COLLISION_H
#define COLLISION_H

#include "../include/glm/glm.hpp"
#include <GLFW/glfw3.h>

namespace collision {
  struct OBB {
    glm::vec3 center;
    glm::vec3 axes[3];
    glm::vec3 halfSize;
  };
}

bool checkAxisOverlap(const collision::OBB& a,const collision::OBB& b, const glm::vec3& axis);
bool checkOBBCollision(const collision::OBB& a, const collision::OBB& b);

#endif
