#include "../include/framework/sensors/standard.h"
#include <algorithm>

Sensor::Sensor() {}

Sensor::Sensor(Entity *_owner)
  : owner(_owner)
{}

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
  return 0;
}

void EdgeSensor::Update(glm::mat4 &transformation) {
  
}

bool EdgeSensor::CheckCollision(EdgeSensor *other) {
  glm::vec3 p1(boundsLeft);
  glm::vec3 p2(boundsRight);
  owner->TransformLocalCoord(p1);
  owner->TransformLocalCoord(p2);

  glm::vec3 p3(other->boundsLeft);
  glm::vec3 p4(other->boundsRight);
  other->owner->TransformLocalCoord(p3);
  other->owner->TransformLocalCoord(p4);
  
  if (p1[0] == p2[0]) p1[0] = p2[0] + 0.0001;
  if (p3[0] == p4[0]) p3[0] = p4[0] + 0.0001;

  float A1 = (p1[1] - p2[1]) / (p1[0] - p2[0]);
  float A2 = (p3[1] - p4[1]) / (p3[0] - p4[0]);

  if (p1[0] == p3[0] && p1[0] == p4[0] && p1[0] == p2[0] ) {
    return false;
  }
  if (A1 == A2) {
    return false;
  }

  float b1 = p1[1] - A1 * p1[0];
  float b2 = p3[1] - A2 * p3[0];

  float Xa = (b2 - b1) / (A1 - A2);

  if ( (Xa < std::max<float>(std::min<float>(p1[0], p2[0]), std::min<float>(p3[0], p4[0]) )) ||
     ( Xa > std::min<float>(std::max<float>(p1[0], p2[0]), std::max<float>(p3[0], p4[0]) )) ) {
    return false; // intersection is out of bound
  }
  else return true;
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
