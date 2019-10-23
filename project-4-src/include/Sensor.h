#ifndef SENSOR_DEF
#define SENSOR_DEF

#include "standard.h"
#include "Entity.h"

class Sensor {
 public:
  Sensor();
  virtual int GetState() = 0;
 protected:
  Entity *owner;
  int state;
};

#endif
