#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/norm.hpp"

#include "../include/config.h"
#include "../include/collision.h"


bool setupOBBCollision(const glm::quat& orientationA,
                       const glm::quat& orientationB,
                       const glm::vec3& positionA,
                       const glm::vec3& positionB,
                       const glm::vec3& scaleA,
                       const glm::vec3& scaleB,
                       const glm::vec2& errorScale){
  collision::OBB a;
  collision::OBB b;

  glm::vec3 aRight = glm::normalize(orientationA * global::localRight);
  glm::vec3 aUp = glm::normalize(orientationA * global::localUp);
  glm::vec3 aFront = glm::normalize(orientationA * global::localFront);

  glm::vec3 bRight = glm::normalize(orientationB * global::localRight);
  glm::vec3 bUp = glm::normalize(orientationB * global::localUp);
  glm::vec3 bFront = glm::normalize(orientationB * global::localFront);

  a.axes[0] = aRight;
  a.axes[1] = aUp;
  a.axes[2] = aFront;
  a.center = positionA;
  a.halfSize = scaleA * (0.5f + errorScale.x);

  b.axes[0] = bRight;
  b.axes[1] = bUp;
  b.axes[2] = bFront;

  b.center = positionB;
  b.halfSize = scaleB * (0.5f + errorScale.y);

  return checkOBBCollision(a,b);

}

bool checkAxisOverlap(const collision::OBB& a,const collision::OBB& b, const glm::vec3& axis){
  
  glm::vec3 diff = b.center - a.center;

  float rA = 
    a.halfSize.x * fabs(glm::dot(axis, a.axes[0])) +
    a.halfSize.y * fabs(glm::dot(axis, a.axes[1])) +
    a.halfSize.z * fabs(glm::dot(axis, a.axes[2]));

  float rB =
    b.halfSize.x * fabs(glm::dot(axis, b.axes[0])) +
    b.halfSize.y * fabs(glm::dot(axis, b.axes[1])) +
    b.halfSize.z * fabs(glm::dot(axis, b.axes[2]));

  float dist = fabs(glm::dot(diff,axis));

  return dist <= (rA + rB) ;
}

bool checkOBBCollision(const collision::OBB& a, const collision::OBB& b){
  glm::vec3 axes[15];
  int axisCount = 0;

  for (int i = 0;i < 3;i++){
    axes[axisCount++] = a.axes[i];
  }
  for (int i = 0;i < 3;i++){
    axes[axisCount++] = b.axes[i];
  }
  for (int i = 0;i < 3;i++){
    for (int j = 0;j < 3;j++){
      glm::vec3 axis = glm::cross(a.axes[i],b.axes[j]);
      axes[axisCount++] = axis;
    }
  }

  for (int i = 0; i < axisCount; i++){
    glm::vec3 axis = axes[i];
    if (glm::length2(axes[i]) < 1e-6f){
      axis = glm::vec3(1e-4f,0.0f,0.0f);
    }

    if (!checkAxisOverlap(a,b,glm::normalize(axis))){
      return false;

    }
  }


  return true;
}


bool checkRayOBBCollision(const glm::vec3& rayOrigin,const glm::vec3& rayDir,const glm::mat4 model,glm::vec3& worldHit){
  glm::mat4 invModel = glm::inverse(model);

  glm::vec3 rayOriginLocal = glm::vec3(invModel * glm::vec4(rayOrigin,1.0));
  glm::vec3 rayDirLocal = glm::normalize(glm::vec3(invModel * glm::vec4(rayDir,1.0)));

  glm::vec3 boxMin(-0.6f);
  glm::vec3 boxMax(0.6f);


  float hitDist;
  float tNear, tFar;
  bool hit = checkRayAABBCollision(rayOriginLocal,rayDirLocal,boxMin,boxMax,tNear,tFar);
  if (hit && tNear >= 0.0f){
    hitDist = tNear;
    glm::vec3 localHit = rayOriginLocal + rayDirLocal * hitDist;
    worldHit = glm::vec3(model * glm::vec4(localHit,1.0f));
    return true;
  }

  return false;
}


bool checkRayAABBCollision(const glm::vec3& rayOrigin, const glm::vec3& rayDir,const glm::vec3& boxMin,const glm::vec3& boxMax,float &tNear,float &tFar){
  tNear = -INFINITY;
  tFar = INFINITY;

  for (int i = 0; i < 3; i++){
    if (rayDir[i] == 0.0f){
      if (rayOrigin[i] < boxMin[i] || rayOrigin[i] > boxMax[i]){
        return false;
      }
    }else{
      float t1 = (boxMin[i] - rayOrigin[i]) / rayDir[i];
      float t2 = (boxMax[i] - rayOrigin[i]) / rayDir[i];
      if (t1 > t2){
        float temp = t1;
        t1 = t2;
        t2 = temp;
      }

      tNear = std::max(tNear,t1);
      tFar = std::min(tFar,t2);

      if (tNear > tFar) return false;
      if (tFar < 0) return false;
    }
  }

  return true;
}

bool checkSphereCubeCollsion(const glm::vec3& sphereCenter,
                             float radius,
                             const glm::quat& cubeOrientation,
                             const glm::vec3& cubePosition,
                             const glm::vec3& cubeScale,
                             float errorScale){

  glm::vec3 dir = sphereCenter - cubePosition;
  glm::vec3 localDir = glm::inverse(cubeOrientation) * dir;


  glm::vec3 halfSize = cubeScale * (0.5f + errorScale);

  glm::vec3 closest = glm::clamp(localDir,-halfSize,halfSize);

  float dist2 = glm::dot(sphereCenter - closest,sphereCenter - closest);
  return dist2 < radius * radius;

  return false;
}

