#ifndef EDGE_DEF
#define EDGE_DEF

#include "standard.h"
#include "Sensor.h"
#include "VertexSensor.h"

class EdgeSensor : public Sensor {
public:
  ProximitySensor();
  ProximitySensor(glm::vec3 _boundsLeft, glm::vec3 _boundsRight, Entity *belongsTo);
  bool CheckCollision(EdgeSensor sensor);
  bool CheckCollision(VertexSensor sensor);
  int GetState();
  glm::vec3 boundsLeft;
  glm::vec3 boundsRight;
  float m;
  float A;
  float B;
  float C;
}

#endif
