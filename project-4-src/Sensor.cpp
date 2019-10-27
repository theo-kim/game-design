#include "include/Sensor.h"
#include "include/ProximitySensor.h"
#include "include/VertexSensor.h"
#include "include/EdgeSensor.h"

Sensor::Sensor() {}

Sensor::Sensor(Entity *_owner)
  : owner(_owner)
{}

ProximitySensor::ProximitySensor() {}

int ProximitySensor::GetState() {
  return 0;
}

EdgeSensor::EdgeSensor() {}

int EdgeSensor::GetState() {
  return 0;
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
