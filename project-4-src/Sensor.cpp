#include "include/Sensor.h"
#include "include/ProximitySensor.h"
#include "include/VertexSensor.h"
#include "include/EdgeSensor.h"

Sensor::Sensor() {}

ProximitySensor::ProximitySensor() {}

int ProximitySensor::GetState() {
  return 0;
}

EdgeSensor::EdgeSensor() {}

int EdgeSensor::GetState() {
  return 0;
}

VertexSensor::VertexSensor() {}

int VertexSensor::GetState() {
  return 0;
}
