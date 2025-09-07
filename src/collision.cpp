#define GLM_ENABLE_EXPERIMENTAL
#include "../include/glm/gtx/norm.hpp"
#include "../include/collision.h"

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
