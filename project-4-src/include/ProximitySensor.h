#ifndef PROXIMITY_DEF
#define PROXIMITY_DEF

#include "standard.h"
#include "Sensor.h"

class ProximitySensor : public Sensor {
public:
  ProximitySensor();
  ProximitySensor(glm::vec3 _loc, float _threshold, Entity *belongsTo);
  bool CheckCollision(ProximitySensor sensor);
  int GetState();
  glm::vec3 loc;
  float threshold;
}

#endif
