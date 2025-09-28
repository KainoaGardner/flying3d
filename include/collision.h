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


bool setupOBBCollision(const glm::quat& orientationA,
                       const glm::quat& orientationB,
                       const glm::vec3& positionA,
                       const glm::vec3& positionB,
                       const glm::vec3& scaleA,
                       const glm::vec3& scaleB,
                       const glm::vec2& errorScale);

bool checkAxisOverlap(const collision::OBB& a,const collision::OBB& b, const glm::vec3& axis);
bool checkOBBCollision(const collision::OBB& a, const collision::OBB& b);
bool checkRayOBBCollision(const glm::vec3& rayOrigin,const glm::vec3& rayDir,const glm::mat4 model,glm::vec3 &worldHit);
bool checkRayAABBCollision(const glm::vec3& rayOrigin,
                           const glm::vec3& rayDir,
                           const glm::vec3& boxMin,
                           const glm::vec3& boxMax,
                           float &tNear,float &tFar);

bool checkSphereCubeCollsion(const glm::vec3& sphereCenter,
                             float radius,
                             const glm::quat& cubeOrientation,
                             const glm::vec3& cubePosition,
                             const glm::vec3& cubeScale,
                             float errorScale);



#endif
