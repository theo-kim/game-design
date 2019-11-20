#include "../include/framework/sensors/standard.h"
#include <algorithm>

Sensor::Sensor() {}

Sensor::Sensor(Entity *_owner)
  : owner(_owner),
    state(0)
{}

void Sensor::DecrementState() {
  state = 0;
}

ProximitySensor::ProximitySensor() {}

ProximitySensor::ProximitySensor(glm::vec3 _loc, float _thresh, Entity *_owner)
  : loc(_loc),
    threshold(std::pow(_thresh, 2)),
    Sensor(_owner) {}

int ProximitySensor::GetState() {
  return 0;
}

void ProximitySensor::Update(glm::mat4 &transformation) {
  
}

bool ProximitySensor::CheckCollision(ProximitySensor *with) {;
  glm::vec3 newLoc(loc);
  owner->TransformLocalCoord(newLoc);
  glm::vec3 newLocOther(with->loc);
  with->owner->TransformLocalCoord(newLocOther);
  float distance = glm::compAdd(glm::pow(newLoc - newLocOther, glm::vec3(2.0f)));
  if (distance < threshold && distance < with->threshold) {
    return true;
  }
  return false;
}

EdgeSensor::EdgeSensor() {}
EdgeSensor::EdgeSensor(glm::vec3 _boundsLeft, glm::vec3 _boundsRight, Entity *belongsTo)
  : Sensor(belongsTo),
    boundsLeft(_boundsLeft),
    boundsRight(_boundsRight) {}

int EdgeSensor::GetState() {
  return state;
}

void EdgeSensor::Update(glm::mat4 &transformation) {
  
}

bool EdgeSensor::CheckCollision(EdgeSensor *other) {
  // state = 0;
  // other->state = 0;
  glm::vec3 p1(boundsLeft);
  glm::vec3 p2(boundsRight);
  owner->TransformLocalCoord(p1);
  owner->TransformLocalCoord(p2);

  glm::vec3 p3(other->boundsLeft);
  glm::vec3 p4(other->boundsRight);
  other->owner->TransformLocalCoord(p3);
  other->owner->TransformLocalCoord(p4);
  
  if (p1[0] == p3[0] && p1[0] == p4[0] && p1[0] == p2[0] ) {
    other->state = 1;
    state = 1;
    return true;
  }

  float Xa = 0;

  // Check if the lines maybe intersect?
  //std::min<float>(p3[0], p4[0]) ||  > std::max<float>(p3[0], p4[0])

  // Check for vertical lines
  if (p1[0] == p2[0] && p3[0] == p4[0]) {
    if (p1[0] != p3[0]) return false; // Not colinear
    if (std::max<float>(p1[1], p2[1]) < std::min<float>(p3[1], p4[1]) ||
        std::min<float>(p1[1], p2[1]) > std::max<float>(p3[1], p4[1])) {
          return false;
        }
    other->state = 1;
    state = 1;
    return true;
  }
  else if (p1[0] == p2[0]) {
    Xa = p1[0];
    float m = (p3[1] - p4[1]) / (p3[0] - p4[0]);
    float b = p3[1] - m * p3[0];
    float y = m * Xa + b;
    if (Xa < std::min<float>(p3[0], p4[0]) || Xa > std::max<float>(p3[0], p4[0])) return false;
    if (y < std::min<float>(p1[1], p2[1]) || y > std::max<float>(p1[1], p2[1])) return false;
    other->state = 1;
    state = 1;
    return true;
  }
  else if (p3[0] == p4[0]) {
    Xa = p3[0];
    float m = (p1[1] - p2[1]) / (p1[0] - p2[0]);
    float b = p1[1] - m * p1[0];
    float y = m * Xa + b;
    if (Xa < std::min<float>(p1[0], p2[0]) || Xa > std::max<float>(p1[0], p2[0])) return false;
    if (y < std::min<float>(p3[1], p4[1]) || y > std::max<float>(p3[1], p4[1])) return false;
    other->state = 1;
    state = 1;
    return true;
  }
  // Otherwise
  float A1 = (p1[1] - p2[1]) / (p1[0] - p2[0]);
  float A2 = (p3[1] - p4[1]) / (p3[0] - p4[0]);

  if (A1 == A2) {
    return false;
  }

  float b1 = p1[1] - A1 * p1[0];
  float b2 = p3[1] - A2 * p3[0];

  Xa = (b2 - b1) / (A1 - A2);
  if ( (Xa < std::max<float>(std::min<float>(p1[0], p2[0]), std::min<float>(p3[0], p4[0]) )) ||
     ( Xa > std::min<float>(std::max<float>(p1[0], p2[0]), std::max<float>(p3[0], p4[0]) )) ) {
    return false; // intersection is out of bound
  }
  else {
    other->state = 1;
    state = 1;
    std::cout << "5" << std::endl;
    return true;
  }
}

VertexSensor::VertexSensor() {}

VertexSensor::VertexSensor(glm::vec3 _loc, Entity *belongsTo)
  : Sensor(belongsTo),
    loc(_loc)
{
  
}

glm::vec3 VertexSensor::GetLoc() {
  return loc;
}

int VertexSensor::GetState() {
  return 0;
}

void VertexSensor::Update(glm::mat4 &transformation) {

}
