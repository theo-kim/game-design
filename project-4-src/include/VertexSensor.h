#ifndef PROXIMITY_DEF
#define PROXIMITY_DEF

#include "standard.h"
#include "Sensor.h"
#include "EdgeSensor.h"

class VertexSensor : public Sensor {
public:
  VertexSensor();
  VertexSensor(glm::vec3 _loc, Entity *belongsTo);
  bool CheckCollision(VertexSensor sensor);
  bool CheckCollision(EdgeSensor sensor);
  int GetState();
  glm::vec3 loc;
}

#endif
