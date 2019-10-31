#include "include/Sensor.h"
#include "include/ProximitySensor.h"
#include "include/VertexSensor.h"
#include "include/EdgeSensor.h"

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

int EdgeSensor::GetState() {
  return 0;
}

void EdgeSensor::Update(glm::mat4 &transformation) {

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
