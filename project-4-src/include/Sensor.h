#ifndef SENSOR_DEF
#define SENSOR_DEF

#include "standard.h"
#include "Entity.h"

class Sensor {
 public:
  Sensor();
  Sensor(Entity *owner);
  virtual int GetState() = 0;
  virtual void Update(glm::mat4 &transformation) = 0;
 protected:
  Entity *owner;
  int state;
};

#endif
